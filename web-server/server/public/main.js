const sockets = io();

let btnTurnOnFanMainRoom = document.querySelector("#btnTurnOnFanMainRoom");
let btnTurnOffFanMainRoom = document.querySelector("#btnTurnOffFanMainRoom");
let btnTurnOnFanBasicRoom = document.querySelector("#btnTurnOnFanBasicRoom");
let btnTurnOffFanBasicRoom = document.querySelector("#btnTurnOffFanBasicRoom");
let btnTurnOnSprinklers = document.querySelector("#btnTurnOnSprinklers");
let btnTurnOffSprinklers = document.querySelector("#btnTurnOffSprinklers");
let temperature = document.querySelector("#temperature");
let stateDoorGarden = document.querySelector("#stateDoorGarden");
let indicationsDoorGarden = document.querySelector("#indicationsDoorGarden");
let stateDoorGarage = document.querySelector("#stateDoorGarage");
let indicationsDoorGarage = document.querySelector("#indicationsDoorGarage");
let alertTemperature = document.querySelector("#alertTemperature");
let stateSprinklers = document.querySelector("#stateSprincklers");
let stateMainRoom = document.querySelector("#stateMainRoom");
let stateBasicRoom = document.querySelector("#stateBasicRoom");

btnTurnOnFanMainRoom.addEventListener("click", () => {
  sockets.emit("serial", "A");
});

btnTurnOffFanMainRoom.addEventListener("click", () => {
  sockets.emit("serial", "B");
});

btnTurnOnFanBasicRoom.addEventListener("click", () => {
  sockets.emit("serial", "C");
});

btnTurnOffFanBasicRoom.addEventListener("click", () => {
  sockets.emit("serial", "D");
});

btnTurnOnSprinklers.addEventListener("click", () => {
  sockets.emit("serial", "E");
});

btnTurnOffSprinklers.addEventListener("click", () => {
  sockets.emit("serial", "F");
});

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

  if (obj.sprinklers == 0) {
    stateSprinklers.innerHTML = "Apagado";
  } else {
    stateSprinklers.innerHTML = "Encendido";
  }

  if (obj.mainRoom == 0) {
    stateMainRoom.innerHTML = "Apagado";
  } else {
    stateMainRoom.innerHTML = "Encendido";
  }

  if (obj.basicRoom == 0) {
    stateBasicRoom.innerHTML = "Apagado";
  } else {
    stateBasicRoom.innerHTML = "Encendido";
  }
});

let dataTemperature;
sockets.on("temp", function (data) {
  if (dataTemperature != data) {
    dataTemperature = data;
    let temperature = [];
    let date = [];
    let dataTemp = data.split("\n");
    for (let i = 0; i < dataTemp.length - 1; i++) {
      let temp = dataTemp[i].split("|");
      temperature.push(temp[0]);
      date.push(temp[1]);
    }
    var ctx = document.getElementById("chartTemperature").getContext("2d");
    var myChart = new Chart(ctx, {
      type: "line",
      data: {
        labels: date,
        datasets: [
          {
            label: "Temperatura",
            data: temperature,
            backgroundColor: [
              "rgba(255, 99, 132, 0.2)",
              "rgba(54, 162, 235, 0.2)",
              "rgba(255, 206, 86, 0.2)",
              "rgba(75, 192, 192, 0.2)",
              "rgba(153, 102, 255, 0.2)",
              "rgba(255, 159, 64, 0.2)",
            ],
            borderColor: [
              "rgba(255, 99, 132, 1)",
              "rgba(54, 162, 235, 1)",
              "rgba(255, 206, 86, 1)",
              "rgba(75, 192, 192, 1)",
              "rgba(153, 102, 255, 1)",
              "rgba(255, 159, 64, 1)",
            ],
            borderWidth: 1,
          },
        ],
      },
      options: {
        scales: {
          y: {
            beginAtZero: false,
          },
        },
      },
    });
  }
});
