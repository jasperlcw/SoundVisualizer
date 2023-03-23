const dgram = require('dgram');
const express = require('express');
const cors = require('cors');
const http = require('http');
const socketIO = require('socket.io');
const fs = require('fs');
const bodyParser = require('body-parser');
const path = require("path");

// dir to music upload
const saveToDir = path.join(__dirname, "../../../", "final-wav-files");
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
        udpServer.send(message, 0, message.length, bbgPort, '192.168.7.2', (error) => {
                if (error) {
                    console.log("unable to send message: ", message);
                } else {
                    res.sendStatus(200);
                    console.log("uploaded file");
                }
            }
        )
    })
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

const handleMessage = (client) =>{
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
}
