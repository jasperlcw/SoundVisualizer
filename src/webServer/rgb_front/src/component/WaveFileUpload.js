import {useEffect, useState} from 'react';
import axios from 'axios';

const WaveFileUpload = ({ipAddress}) => {

    const [waveFile, setWaveFile] = useState(null);
    const [uploadComplete, setUploadComplete] = useState(true);
    const [uploadText, setUploadText] = useState("upload a file");

    const handleFileChange = (event) =>{
        setUploadText("please upload a file");
        setWaveFile(event.target.files[0]);
    }

    const sendWaveFile = () =>{
        if(!waveFile) return;
        setUploadComplete(false);
        setUploadText("uploading");
        const reader = new FileReader();
        reader.onload = () =>{
            const contents = reader.result;
            const target = `${ipAddress}/upload`;
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
            <div style={{display:"flex", alignItems: "center", justifyContent: "center"}}>
                <input className = "uploadText" type="file" accept="audio/wav" onChange={handleFileChange}/>
                <button className = "uploadText" onClick={sendWaveFile} disabled={!uploadComplete}>Upload</button>
            </div>

            <div>
                <p className = "uploadText" style={{margin: "1%"}}>{uploadText}</p>
            </div>

        </div>
    )
}
export default WaveFileUpload;