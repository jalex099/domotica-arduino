const sockets = io();

let btnSendData = document.querySelector("#btnSendData");
let btnSendDataOff = document.querySelector("#btnSendDataOff");
let temperature = document.querySelector("#temperature");
let stateDoorGarden = document.querySelector("#stateDoorGarden");
let indicationsDoorGarden = document.querySelector("#indicationsDoorGarden");
let stateDoorGarage = document.querySelector("#stateDoorGarage");
let indicationsDoorGarage = document.querySelector("#indicationsDoorGarage");
let alertTemperature = document.querySelector("#alertTemperature");

btnSendData.addEventListener("click",()=>{
  sockets.emit("led", "A")
})

btnSendDataOff.addEventListener("click",()=>{
  sockets.emit("led", "B")
})

sockets.on("data", function (data) {
  //OBTENCION DE LA DATA
  let obj = data;

  //TEMPERATURA
  temperature.innerHTML = `${Math.floor(obj.temperature)} °C `;
  if (obj.temperature > 35) {
    try {
      temperature.classList.remove("text-blue");
      temperature.classList.add("text-red");
      alertTemperature.classList.remove("hidden");
      alertTemperature.classList.add("show");
    } catch {}
  } else {
    try {
      temperature.classList.remove("text-red");
      temperature.classList.add("text-blue");
      alertTemperature.classList.remove("show");
      alertTemperature.classList.add("hidden");
    } catch {}
  }

  //PUERTA DEL PATIO INTERIOR
  if (obj.gardenDoor == 0) {
    stateDoorGarden.innerHTML = "Patio interior: CERRADO";
    indicationsDoorGarden.innerHTML = "";
    stateDoorGarden.classList.remove("text-red");
    stateDoorGarden.classList.add("text-white");
  } else {
    stateDoorGarden.innerHTML = "Patio interior: ABIERTO";
    indicationsDoorGarden.innerHTML =
      "La puerta del patio interior esta abierta, persona pasando en este momento.";
    stateDoorGarden.classList.remove("text-white");
    stateDoorGarden.classList.add("text-red");
  }

  if (obj.garageDoor == 0) {
    stateDoorGarage.innerHTML = "Cochera: CERRADA";
    indicationsDoorGarage.innerHTML = "";
    stateDoorGarage.classList.remove("text-red");
    stateDoorGarage.classList.add("text-white");
  } else {
    stateDoorGarage.innerHTML = "Cochera: ABIERTA";
    indicationsDoorGarage.innerHTML =
      "La puerta de la cochera esta abierta, persona pasando en este momento.";
    stateDoorGarage.classList.remove("text-white");
    stateDoorGarage.classList.add("text-red");
  }
});
