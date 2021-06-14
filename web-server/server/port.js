const SerialPort = require("serialport");

const port = new SerialPort(
    "COM5",
    {
      baudRate: 9600,
    },
    false
  );

  module.exports = port