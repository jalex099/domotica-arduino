const SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;

const parser = new Readline();
const mySerial = new SerialPort("COM2",{
    baudRate:9600
},false)

/*mySerial.on("open",()=>{
console.log("Puerto serial abierto")
})*/

mySerial.on("data",(data)=>{
    console.log(data)
    })