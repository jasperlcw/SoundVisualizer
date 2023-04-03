import logo from './logo.svg';
import './App.css';
import {useEffect, useState, useRef} from 'react';
import axios from 'axios';
// socket
import io from 'socket.io-client';

// components
import AudioVisualiser from "./component/AudioVisualiser";
import BrightnessBar from './component/BrightnessBar';
import WaveFileUpload from './component/WaveFileUpload'

// pages 
import LedOffPage from './pages/LedOffPage'
import DisplayTimePage from './pages/DisplayTimePage'
import AudioVisualiserPage from './pages/AudioVisualiserPage'

// functions
import {createNewboard} from './functions/board';

const LED_Mode = {
  0 : "LED OFF",
  1 : "Current Time",
  2 : "Audio Visualiser",
};


let socket = io('http://192.168.7.2:8080');
function App() {

  const [messages, setMessage] = useState("");
  const [spectrum, setSpectrum] = useState([]);
  const [brightness, setBrightness] = useState([]);
  const [board, setBoard] = useState([]);
  const [timeBoard, setTimeBoard] = useState([]);
  const [screen, setScreen] = useState([]);
  const [mode, setMode] = useState("Do you guys not have phone?");
  const [ipAddress, setIpAddress] = useState('http://192.168.7.2:8080');
  const [confirmIp, setConfirmIp] = useState('');

  const [errorTimer, setErrorTimer] = useState(0);
  const [ErrorInterval, setErrorInterval] = useState(null);


  const [errorMessage, setErrorMessage] = useState("Can't connect to Node Js Server");

  const canvasRef = useRef(null);

  // received a message from bbg
  useEffect(() => {
    socket.on('message', (msg) =>{
        setMessage(msg);
    })
  },[errorMessage, socket, ErrorInterval])

  useEffect(() =>{
    if(socket){
      socket.on("disconnect", () => {
        setErrorMessage("Can't connect to Node JS Server");
        setMode("-1");
      });
      socket.on("connect", () => {
        setErrorMessage("");
        console.log(`connected to: ${ipAddress}`)
      });
    }
  },[errorMessage, socket])

  //initial board
  useEffect(()=>{
    let newBoard = createNewboard();
    setBoard(newBoard);
    setTimeBoard(newBoard);
  }, [])


  //send getMode to bbg
  useEffect(()=>{
    const Interval = setInterval(()=>{
      sendMessage("getMode");
    }, 1000)
    return () => clearInterval(Interval);
  },[errorMessage, socket, ErrorInterval])

  useEffect(()=>{
    setNewCommend();
  },[messages])

    //send a message to bbg
  const sendMessage = (cmd) => {
      // const response = socket.emit('connection', "testConn")
    socket.emit('message', cmd);
  }

  // determine what commands base on response
  const setNewCommend = () => {
    let cmd = messages.split(" ");
    // console.log(cmd[0])
    if (cmd[0] === "spectrum"){
      try{
        setSpectrum(JSON.parse(cmd[1]));
        setErrorMessage("");
      }catch(error){
        console.log(error);
      }
    }
    else if (cmd[0] === "brightness") {
      try {
        setBrightness(cmd[1]);
        setErrorMessage("");
      } catch(error) {
        console.log(error);
      }
    }
    else if (cmd[0] === "mode") {
      try {
        setMode(cmd[1]);
        setErrorMessage("");
      } catch(error) {
        console.log(error);
      }
    }
    else if (cmd[0] === "screen") {
      try {
        setScreen(JSON.parse(cmd[1]));
        setErrorMessage("");
      } catch(error) {
        console.log(error);
      }
    }
    else if (cmd[0] === "error") {
      try {
        setErrorMessage("can't connect to BBG Server");
      } catch(error) {
        console.log(error);
      }
    }
  }

  //update IP
  const updateServerIp = () =>{

    if(socket){
      socket.disconnect();
    }
    socket = io(ipAddress);
    setConfirmIp(ipAddress);
  }

  const handleIpChange = (e) =>{
    setIpAddress(e.target.value);
  }


  return (
    <div className="App">
      <header className="App-header">
        {/*<img src={logo} className="App-logo" alt="logo" />*/}
        <p className = "titleText">TEAM <span style = {{ color: 'red' }}>R</span><span style = {{ color: 'green'}}>G</span><span style = {{ color: 'blue' }}>B</span></p>

        <div style={{ display: 'flex' , width: '80%', justifyContent: "center", alignItems: "center"}}>
          <div style={{ flex: 1, alignItems: "flex-end"}}>
            <button className = "custom-button" onClick={() => sendMessage("setPreviousMode")}> {"<"} </button>
          </div>
          <div style={{flex: 0.8}}>
            <p className = "modeText">{LED_Mode[mode]}</p>
          </div>
          <div style={{ flex: 1, alignItems: "flex-start"}}>
            <button className = "custom-button" onClick={() => sendMessage("setNextMode")}> {">"} </button>
          </div>
        </div>
      </header>

      <div className="App-main">
        <div>
            <h1 style={{color:'red'}}>{errorMessage}</h1>
        </div>
        {
          mode === "0" ? (<LedOffPage/>) : 
          mode === "1" ? (<DisplayTimePage timeBoard = {timeBoard} setTimeBoard = {setTimeBoard} screen = {screen} sendMessage = {sendMessage} canvasRef = {canvasRef}/>) : 
          mode === "2" ? (<AudioVisualiserPage spectrum = {spectrum} board = {board} setBoard = {setBoard} brightness = {brightness} sendMessage = {sendMessage} canvasRef = {canvasRef}/>):
          <div>
            <h1> Connection To BBG manually </h1>
            <input className ="modeText" type = "text" value = {ipAddress} onChange = {handleIpChange}></input>
            <button className="modeText" style={{marginTop:"1%"}} onClick = {updateServerIp}>Connect</button>
          </div>
        }
      </div>
    </div>
  );
}

export default App;
