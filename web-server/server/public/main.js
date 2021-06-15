const sockets = io();

sockets.on("data",function (data) {
    let obj = data;

    const temp = document.getElementById('temperature')
    temp.innerHTML = `${Math.floor(obj.temperature)} °C ` 
})

let btnSendData = document.querySelector("#btnSendData");

btnSendData.addEventListener("click",()=>{
  sockets.emit("led-on", 1)
})