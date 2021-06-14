const sockets = io();

sockets.on("temp",function (data) {
    const temp = document.getElementById('temperature')
    temp.innerHTML = `${data} °C` 
})

let btnSendData = document.querySelector("#btnSendData");

btnSendData.addEventListener("click",()=>{
  sockets.emit("led-on", 1)
})