const dgram = require('dgram');
const express = require('express');
const cors = require('cors');
const http = require('http');
const socketIO = require('socket.io');

const app = express();
app.use(cors());

const server = http.createServer(app);
const socket = socketIO(server, {
    cors: {
        origin: '*',
    }
});

const bbgPort = 12345
const webServerPost = 8080
const udpServer = dgram.createSocket('udp4');
let messageCount = 0;

udpServer.on('error', (err) => {
    console.log("UDP Server Error: ");
    console.log(err);
})
udpServer.on('message', (msg, info) =>{
    console.log(`received: ${msg} from BBG: ${info.address}:${info.port}`);
    socket.emit('message', msg.toString());
    messageCount++;
    console.log(`message Count: ${messageCount}`);
})

udpServer.on('listening', () => {
    console.log(`UDP server listening on port ${12346}`);
})

udpServer.bind(12346);

socket.on('connection', (client) =>{
    client.on('message', (data) =>{
        // console.log(`Received message from web server: ${data}`);
        const message = Buffer.from(data, 'utf8');
        udpServer.send(message, 0, message.length, bbgPort, '192.168.7.2',(error) =>{
            if(error){
                console.log("unable to send message: ", message);
            }
            else{
                console.log("sent: ", data);
            }
            }
        )
    });
})

server.listen(webServerPost, () => {
    console.log(`Web server listening on port ${webServerPost}`)
})

