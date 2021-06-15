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

io.on("connection",(socket)=>{
  socket.on('led-on',(varLed) =>{
    console.log(varLed);
  })
})
//emit por socket
parser.on("data", (data) => {
  try{
    console.log(data);
    let json = JSON.parse(data);
    io.emit('data',json)
  } catch(err){
    console.log("Error" + err.toString());
  }
  
  /*port.write('main screen turn on', function(err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('message written')
  })*/
});

port.on("error", (err) => {
  console.log(err);
});
