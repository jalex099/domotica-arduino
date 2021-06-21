const http = require("http");
const express = require("express");
const { Server } = require("socket.io");
const fs = require("fs");
const dateFormat = require("dateformat");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static(__dirname + "/public"));

server.listen(3000, function () {
  console.log("server listening on port");
});

//Serial comunication
const SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;

const port = require("./port");

const parser = port.pipe(new Readline({ delimiter: "\r\n" }));

parser.on("open", () => {
  console.log("Puerto serial abierto");
});

//VARIABLES
let pastTemperature=0;
let pastDigitalDoor=0;



//sockets events
io.on("connection", (socket) => {
  socket.on("serial", (data) => {
    try {
      port.write(data);
    } catch {}
  });
});
//emit por socket
parser.on("data", (data) => {
  try {
    let json = JSON.parse(data);
    io.emit("data", json);
    fs.readFile("temperature.txt", "utf-8", (err, dataS) => {
      if (err) {
      } else {
        io.emit("temp", dataS);
      }
    });
    fs.readFile("digitalDoor.txt", "utf-8", (err, dataD) => {
      if (err) {
      } else {
        io.emit("securityReport", dataD);
      }
    });
    var date = dateFormat(new Date(), "yyyy-mm-dd h:MM:ss");
    let temperature = Math.floor(json.temperature);
    let digitalDoor = json.digitalDoor;
    if(!isNaN(temperature)){
      if (temperature != pastTemperature) {
        fs.appendFile(
          "./temperature.txt",
          temperature + "|" + date + "\n",
          function (err) {
            if (err) {
              return console.log(err);
            }
          }
        );
        pastTemperature = temperature;
      }
    }
    if(!isNaN(digitalDoor)){
      if (digitalDoor != pastDigitalDoor) {
        if (digitalDoor > 0) {
          if (digitalDoor == 1) {
            fs.appendFile(
              "./digitalDoor.txt",
              "Abierta correctamente" + "|" + date + "\n",
              function (err) {
                if (err) {
                  return console.log(err);
                }
              }
            );
          } else if (digitalDoor == 2) {
            fs.appendFile(
              "./digitalDoor.txt",
              "Intento fallido" + "|" + date + "\n",
              function (err) {
                if (err) {
                  return console.log(err);
                }
              }
            );
          }
          pastDigitalDoor = digitalDoor;
        } else {
          pastDigitalDoor = digitalDoor;
        }
      }
    }
    
  } catch (err) {
    console.log("Error" + err.toString());
  }
});

port.on("error", (err) => {
  console.log(err);
});


