import {useRef, useEffect} from 'react';

import './Page.css';

// components
import AudioVisualiser from "../component/AudioVisualiser";
import BrightnessBar from '../component/BrightnessBar';
import WaveFileUpload from '../component/WaveFileUpload'
import VolumeController from '../component/VolumeController'

// functions
import {createNewboard} from '../functions/board';


const AudioVisualiserPage = ({spectrum, board, setBoard, brightness, sendMessage, canvasRef, volume, volumeControl, setVolumeControl}) => {

    //send getSpectrum to bbg
    useEffect(()=>{
    const Interval = setInterval(()=>{
        sendMessage("getSpectrum");
    }, 100)
    return () => clearInterval(Interval);
    },[])

    //send getBrightness to bbg
    useEffect(()=>{
    const Interval = setInterval(()=>{
        sendMessage("getBrightness");
    }, 1000)
    return () => clearInterval(Interval);
    },[])

    //initial board
    useEffect(()=>{
        const newBoard = createNewboard();
        setBoard(newBoard);
    }, [])

    return (
        <div className="page-header">
            <div>
                <AudioVisualiser spectrum = {spectrum} board = {board} setBoard = {setBoard} canvasRef ={canvasRef}/>
            </div>

            <div className = "audioContainer">
                <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={1000} height={500}/>
            </div>

            <div style={{width: "60%", height: "50%", margin: "2% 1% 1% 1%", }}>
                <VolumeController sendMessage = {sendMessage} volume = {volume} volumeControl = {volumeControl} setVolumeControl = {setVolumeControl}/>
            </div>
            {/* <div>
                <BrightnessBar brightness = {brightness}/>
            </div> */}

            <div style={{width:"60%", margin: "1% 2% 1% 2%"}}>
                <WaveFileUpload />
            </div>
        </div>
    )
}
export default AudioVisualiserPage;