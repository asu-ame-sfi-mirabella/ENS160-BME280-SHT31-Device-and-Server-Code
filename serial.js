// read serial port and print to console

var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

var portName = '/dev/ttyACM0';
var sp = new SerialPort(portName, {
    baudRate: 115200,
    parser: serialport.parsers.readline('\n')
});

sp.on('data', function(input) {
    console.log(input);
});

