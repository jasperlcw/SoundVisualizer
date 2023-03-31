import {useEffect, useState} from 'react';
import axios from 'axios';

const WaveFileUpload = () => {

    const [waveFile, setWaveFile] = useState(null);
    const [uploadComplete, setUploadComplete] = useState(true);
    const [uploadText, setUploadText] = useState("upload a file");

    const handleFileChange = (event) =>{
        setUploadText("upload a file");
        setWaveFile(event.target.files[0]);
    }

    const sendWaveFile = () =>{
        if(!waveFile) return;
        setUploadComplete(false);
        setUploadText("uploading");
        const reader = new FileReader();
        reader.onload = () =>{
            const contents = reader.result;
            const target = 'http://192.168.7.2:8080/upload';
            const headers = {
                'Content-Type': 'audio/wav'
            };
            //create buffer
            const data = new Uint8Array(contents)
            axios.post(target, data, {headers})
                .then(res => {
                    console.log(res);
                    setUploadText("uploaded");
                    setUploadComplete(true);
                })
                .catch(err => {
                    console.log(err);
                    setUploadComplete(true);
                });
        }
        reader.readAsArrayBuffer(waveFile);
    }

    return (
        <div>
            <input type="file" accept="audio/wav" onChange={handleFileChange}/>
            <button onClick={sendWaveFile} disabled={!uploadComplete}>Upload</button>
            <span style={{marginLeft: "1%"}}>{uploadText}</span>
        </div>
    )
}
export default WaveFileUpload;