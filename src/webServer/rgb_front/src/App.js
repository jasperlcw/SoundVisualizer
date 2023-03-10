import logo from './logo.svg';
import './App.css';
import {useEffect, useState} from 'react';

import io from 'socket.io-client';
import AudioVisualiser from "./component/AudioVisualiser";

const socket = io('http://192.168.7.2:8080');
function App() {

  const [messages, setMessage] = useState("");
  const [spectrum, setSpectrum] = useState([]);

  // set the message state when it received a message from bbg
  useEffect(() => {
    socket.on('message', (msg) =>{
        setMessage(msg);
        console.log("setMessage")
    })
  },[])

  //send getSpectrum to bbg
  useEffect(()=>{
    const getSpectrumInterval = setInterval(()=>{
      sendMessage("getSpectrum\n");
    }, 100)
    return () => clearInterval(getSpectrumInterval);
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

        <h1>UDP Messages</h1>
        <p>{messages}</p>

        <div>
          <AudioVisualiser spectrum = {spectrum}/>
        </div>

        <button onClick={() => sendMessage("help")}> click me for help!</button>
      </header>
    </div>
  );
}

export default App;
