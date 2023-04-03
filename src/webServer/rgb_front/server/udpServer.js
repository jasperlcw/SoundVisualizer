const dgram = require('dgram');
const express = require('express');
const cors = require('cors');
const http = require('http');
const socketIO = require('socket.io');
const fs = require('fs');
const bodyParser = require('body-parser');
const path = require("path");

let currentSenderIp = "";

// dir to music upload
const saveToDir = path.join("/tmp/final-wav-files");
console.log(saveToDir)
if (!fs.existsSync(saveToDir)) {
    fs.mkdirSync(saveToDir);
}

//ports
const bbgPort = 12345
const webServerPost = 8080

// udp server
const udpServer = dgram.createSocket('udp4');
udpServer.bind(12346);

//udp server handler
udpServer.on('error', (err) => {
    console.log("UDP Server Error: ");
    console.log(err);
})
udpServer.on('message', (msg, info) =>{
    console.log(`received: ${msg} from BBG: ${info.address}:${info.port}`);
    clearInterval(errorInterval);
    errorInterval = null;
    errorTimer = 0;
    
    socket.emit('message', msg.toString());

    messageCount++;
    console.log(`message Count: ${messageCount}`);

})
udpServer.on('listening', () => {
    console.log(`UDP server listening on port ${12346}`);
})


// express server
const app = express();

app.use(bodyParser.raw({ limit: '50mb', type: 'audio/wav' }));
app.use(bodyParser.json());

app.use(cors());

//upload file promise
const uploadingFile = (file) =>{
    return new Promise((resolve, reject) =>{
        fs.writeFile(path.join(saveToDir, 'uploaded.wav'), file, (error) =>{
            console.log(error);
            if(error) reject(error);
            resolve();
        });
    })
}
//upload file
app.post('/upload',(req, res) => {
    console.log(saveToDir);
    console.log(req.body)
    uploadingFile(req.body).then(()=>{
        const message = Buffer.from("setMusic", 'utf8');
        udpServer.send(message, 0, message.length, bbgPort, 'localhost', (error) => {
                if (error) {
                    console.log("unable to send message: ", message);
                } else {
                    currentSenderIp = req.ip;
                    res.sendStatus(200);
                    console.log("uploaded file");
                }
            }
        )
    })
})

//listen to message
app.post('/message',(req, res) => {
    const message = req.body.data;
    udpServer.send(message, 0, message.length, bbgPort, 'localhost', (error) => {
            if (error) {
                console.log("unable to send message: ", message);
            } else {
                currentSenderIp = req.ip;
                res.sendStatus(200);
                console.log("sent: ", req.body.data);
            }
        }
    )
})

//web server with express//
const server = http.createServer(app);
const socket = socketIO(server, {
    cors: {
        origin: '*',
    }
});


server.listen(webServerPost, () => {
    console.log(`Web server listening on port ${webServerPost}`);
})

//socket
socket.on('connection', (client) =>{
    handleMessage(client);
})

let messageCount = 0;

let errorInterval = null;
let errorTimer = 0;


const handleMessage = (client) =>{
    client.on('message', (data) =>{
        // console.log(`Received message from web server: ${data}`);
        const message = Buffer.from(data, 'utf8');
        udpServer.send(message, 0, message.length, bbgPort, 'localhost',(error) =>{
                if(error){
                    console.log("unable to send message: ", message);
                }
                else{
                    console.log("sent: ", data);
                    if(!errorInterval){
                        errorInterval = setInterval(()=>{
                            if(errorTimer > 20){
                                socket.emit('message', "error");
                            }
                            errorTimer+=1;
                        },100)
                    }
                }
            }
        )
    });
}
