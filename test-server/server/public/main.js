const sockets = io();

sockets.on("temp",function (data) {
    const temp = document.getElementById('temperature')
    temp.innerHTML = `${data} °C` 
})