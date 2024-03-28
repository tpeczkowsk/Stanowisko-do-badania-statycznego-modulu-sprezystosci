const { SerialPort } = require("serialport");
import * as CONST from "./constants.js";

const serial_connect_button = document.querySelector("[data-serial-connect]");
const serial_select = document.getElementById("serial-select");
const serial_status_indicator = document.querySelector("[data-serial-indicator]");
const button_start = document.querySelector("[data-start]");
const button_stop = document.querySelector("[data-stop]");
const status = document.querySelector("[data-status]");
var port = null;
var serial_state = CONST.SERIAL_STATE_CLOSED;
var pollInitialConnectionInterval;

var isPortOpen = false;

// SerialPortMock.binding.createPort("/dev/test");
serial_connect_button.addEventListener("click", connectToSelectedPort);

function parseData(data) {
    const buf = data.toString();
    console.log(`Received: ${buf}`);
    switch (serial_state) {
        // polling for connection
        case 1:
            if (buf == "Pong") {
                clearInterval(pollInitialConnectionInterval);
                serial_state = CONST.SERIAL_STATE_CONNECTED;
                setStatus(1);
                button_start.disabled = false;
                button_stop.disabled = false;
            }
            break;
        // polling for data
        case 2:
            break;
        default:
            break;
    }
}

function connectToSelectedPort() {
    if (isPortOpen == false) {
        const selected_port = serial_select.options[serial_select.selectedIndex];
        port = new SerialPort({
            path: selected_port.value,
            baudRate: 115200,
            dataBits: 8,
            stopBits: 1,
            parity: "none",
        });
        port.on("open", () => {
            handlePortOpen();
            startPollConnection();
            console.log(`port ${port.path} is opened`);
        });
        port.on("error", (err) => {
            console.log(`Port ${port.path} with error ${err}`);
        });
        port.on("close", () => {
            handlePortClose();
            console.log(`port ${port.path} is closed`);
            port = null;
        });
        port.on("data", (data) => {
            parseData(data);
        });
    } else {
        if (port) {
            port.close();
        }
    }
}

async function getSerialPortList() {
    await SerialPort.list().then((ports, err) => {
        console.log(ports);
        ports.forEach((port) => {
            const option = document.createElement("option");
            option.value = port.path;
            option.textContent = port.path;
            serial_select.appendChild(option);
        });
    });
}

function pollInitialConnection() {
    port.write("Ping", () => {
        // port.port.emitData("Pong");
        console.log("Sent");
    });
}

function startPollConnection() {
    pollInitialConnection();
    pollInitialConnectionInterval = setInterval(pollInitialConnection, 1000);
}

function handlePortOpen() {
    isPortOpen = true;
    serial_status_indicator.style.backgroundColor = "var(--green-color)";
    serial_select.disabled = true;
    serial_connect_button.textContent = "Rozłącz";
    setStatus(254);
    serial_state = CONST.SERIAL_STATE_OPENED;
}

function handlePortClose() {
    isPortOpen = false;
    clearInterval(pollInitialConnectionInterval);
    serial_status_indicator.style.backgroundColor = "red";
    serial_select.disabled = false;
    serial_connect_button.textContent = "Połącz";
    button_start.disabled = true;
    button_stop.disabled = true;
    serial_state = CONST.SERIAL_STATE_CLOSED;
    setStatus(255);
}

function setStatus(index) {
    switch (index) {
        // jest kontroler, ale oczekuje na rozpoczecie
        case 1:
            status.textContent = CONST.STATUS_1;
            status.style.backgroundColor = "var(--blue-color)";
            break;
        // brak polaczenia
        case 255:
            status.textContent = CONST.STATUS_255;
            status.style.backgroundColor = "var(--status-grey)";
            break;
        // brak mikrokontrolera
        case 254:
            status.textContent = CONST.STATUS_254;
            status.style.backgroundColor = "var(--status-grey)";
            break;
        default:
            break;
    }
}

setStatus(255);
getSerialPortList();
