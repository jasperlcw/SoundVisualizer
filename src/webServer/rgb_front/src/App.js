import logo from './logo.svg';
import './App.css';
import {useEffect, useState, useRef} from 'react';

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


const socket = io('http://192.168.7.2:8080');
function App() {

  const [messages, setMessage] = useState("");
  const [spectrum, setSpectrum] = useState([]);
  const [brightness, setBrightness] = useState([]);
  const [board, setBoard] = useState([]);
  const [screen, setScreen] = useState([]);
  const [mode, setMode] = useState("Do you guys no have phone?");

  // set the message state when it received a message from bbg
  useEffect(() => {
    socket.on('message', (msg) =>{
        setMessage(msg);
        console.log("setMessage")
    })
  },[])

  //initial board
  useEffect(()=>{
    const newBoard = createNewboard();
    setBoard(newBoard);
  }, [])

  //send getMode to bbg
  useEffect(()=>{
    const Interval = setInterval(()=>{
      sendMessage("getMode");
    }, 1000)
    return () => clearInterval(Interval);
  },[])

  useEffect(()=>{
    setNewCommend();
  },[messages])

  // determine what commands base on response
  const setNewCommend = () => {
    let cmd = messages.split(" ");
    // console.log(cmd[0])
    if (cmd[0] === "spectrum"){
      try{
        setSpectrum(JSON.parse(cmd[1]));
      }catch(error){
        console.log(error);
      }
    }
    else if (cmd[0] === "brightness") {
      try {
        setBrightness(cmd[1]);
      } catch(error) {
        console.log(error);
      }
    }
    else if (cmd[0] === "mode") {
      try {
        setMode(cmd[1]);
      } catch(error) {
        console.log(error);
      }
    }
    else if (cmd[0] === "screen") {
      try {
        setScreen(JSON.parse(cmd[1]));
      } catch(error) {
        console.log(error);
      }
    }
  }

  //send a message to bbg
  const sendMessage = (cmd) => {
    // const response = socket.emit('connection', "testConn");
    const response = socket.emit('message', cmd);
    // console.log(response2);
  }

  return (
    <div className="App">
      <header className="App-header">
        {/*<img src={logo} className="App-logo" alt="logo" />*/}
        <p class = "titleText">TEAM <span style = {{ color: 'red' }}>R</span> <span style = {{ color: 'green'}}>G</span> <span style = {{ color: 'blue' }}>B</span></p>

        <div style={{ display: 'flex' , width: '80%', justifyContent: "center", alignItems: "center"}}>
          <div style={{ flex: 1, alignItems: "flex-end"}}>
            <button class = "custom-button" onClick={() => sendMessage("setPreviousMode")}> {"<"} </button>
          </div>
          <div style={{flex: 0.8}}>
            <p class = "modeText">Current Mode: {mode}</p>
          </div>
          <div style={{ flex: 1, alignItems: "flex-start"}}>
            <button class = "custom-button" onClick={() => sendMessage("setNextMode")}> {">"} </button>
          </div>
        </div>
      </header>

      <div className="App-main">
        {/*<p style={{display: "inline", margin: 0, padding: 0 }}>{messages}</p>*/}
        {
          mode === "0" ? (<LedOffPage/>) : 
          mode === "1" ? (<DisplayTimePage board = {board} setBoard = {setBoard} screen = {screen} sendMessage = {sendMessage}/>) : 
          mode === "2" ? (<AudioVisualiserPage spectrum = {spectrum} board = {board} setBoard = {setBoard} brightness = {brightness} sendMessage = {sendMessage}/>):
          <div></div>
        }
        {/* 
        <div>
          <AudioVisualiser spectrum = {spectrum} board = {board} setBoard = {setBoard} canvasRef ={canvasRef}/>
        </div>

        <div>
            <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/>
        </div>

        <div>
          <BrightnessBar brightness = {brightness}/>
        </div>
        <div style={{width:"60%", margin: "1% 2% 1% 2%"}}>
          <WaveFileUpload />
        </div> */}

        {/* <button onClick={() => sendMessage("setNextMode")}> click me to change mode!</button> */}


      </div>



      
    </div>
  );
}

export default App;
