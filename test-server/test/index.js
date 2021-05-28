const SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;

const parser = new Readline();
const mySerial = new SerialPort("COM1",{
    baudRate:9600
})

mySerial.on("open",()=>{
console.log("Puerto serial abierto")
})

mySerial.on("data",(data)=>{
    console.log(data)
    })