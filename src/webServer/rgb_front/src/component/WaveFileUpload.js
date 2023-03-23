import {useEffect, useState} from 'react';
import axios from 'axios';

const WaveFileUpload = () => {

    const [waveFile, setWaveFile] = useState(null);
    const [uploadComplete, setUploadComplete] = useState(false);
    const [uploadText, setUploadText] = useState("upload a file");

    const handleFileChange = (event) =>{
        setUploadText("upload a file");
        setWaveFile(event.target.files[0]);
    }

    const sendWaveFile = () =>{
        if(!waveFile) return;
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
                })
                .catch(err => console.log(err));

        }
        reader.readAsArrayBuffer(waveFile);
    }

    return (
        <div>
            <input type="file" onChange={handleFileChange} accept='audio/wav'/>
            <button onClick={sendWaveFile}>Upload</button>
            <span style={{marginLeft: "1%"}}>{uploadText}</span>
        </div>
    )
}
export default WaveFileUpload;