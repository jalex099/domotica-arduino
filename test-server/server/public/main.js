const sockets = io();

sockets.on("temp",function (data) {
    const temp = document.getElementById('temperature')
    temp.innerHTML = `${data} °C` 
})

let btnSendData = document.querySelector("#btnSendData");

btnSendData.addEventListener("click",()=>{
    const port = require("../port")
    console.log(port);
    port.write('main screen turn on', function(err) {
        if (err) {
          return console.log('Error on write: ', err.message)
        }
        console.log('message written')
      })
})