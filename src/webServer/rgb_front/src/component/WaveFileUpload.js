import {useEffect, useState} from 'react';
import axios from 'axios';

const WaveFileUpload = () => {

    const [waveFile, setWaveFile] = useState(null);
    const [uploadComplete, setUploadComplete] = useState(false)

    const handleFileChange = (event) =>{
        setWaveFile(event.target.files[0]);
    }

    const sendWaveFile = () =>{
        if(!waveFile) return;
        console.log("uploading")
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
                    setUploadComplete(true);
                })
                .catch(err => console.log(err));

        }
        reader.readAsArrayBuffer(waveFile);
    }

    return (
        <div>
            <input type="file" onChange={handleFileChange} accept='audio/wav'/>
            <button onClick={sendWaveFile}>Upload</button>
            {uploadComplete ? <span>{"uploadComplete!"}</span> : <span style={{marginLeft: "3px"}}>upload a file</span>}

        </div>
    )
}
export default WaveFileUpload;