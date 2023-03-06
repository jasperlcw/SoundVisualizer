import logo from './logo.svg';
import './App.css';
import { useEffect, useState } from 'react';

import io from 'socket.io-client';

const socket = io('http://192.168.7.2:8080');
function App() {

  const [messages, setMessage] = useState([]);

  useEffect(() => {
    socket.on('message', (msg) =>{
      setMessage((message) => [...messages, msg])
    })
  })

  const sendMessage = () => {
    // const response = socket.emit('connection', "testConn");
    const response2 = socket.emit('message', "help\n");
    console.log(response2);
  }

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />

        <h1>UDP Messages</h1>

        <ul>
          {messages.map((msg, index) => (
              <li key={index}>{msg}</li>
          ))}
        </ul>

        <button onClick={sendMessage}> click me for help!</button>
      </header>
    </div>
  );
}

export default App;
