const http = require("http");
const express = require("express");
const {Server} = require("socket.io");


const app = express();
const server = http.createServer(app);
const io = new Server(server)


app.use(express.static(__dirname + "/public"))

server.listen(3000,function () {
    console.log("server listening on port");
})



//Serial comunication
const SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;

const port = require("./port")

const parser = port.pipe(new Readline({ delimiter: "\r\n" }));

parser.on("open", () => {
  console.log("Puerto serial abierto");
});


//emit por socket
parser.on("data", (data) => {
  io.emit('temp',data)
  console.log(data.toString());
});

port.on("error", (err) => {
  console.log(err);
});
