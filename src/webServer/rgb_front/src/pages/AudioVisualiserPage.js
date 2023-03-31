import {useRef, useEffect} from 'react';

import './Page.css';

// components
import AudioVisualiser from "../component/AudioVisualiser";
import BrightnessBar from '../component/BrightnessBar';
import WaveFileUpload from '../component/WaveFileUpload'

// functions
import {createNewboard} from '../functions/board';

// In pixels
const canvasHeight = 400;
const canvasWidth = 800;



const AudioVisualiserPage = ({spectrum, board, setBoard, brightness, sendMessage}) => {

    const canvasRef = useRef(null);

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

            <div>
                <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/>
            </div>

            <div>
                <BrightnessBar brightness = {brightness}/>
            </div>

            <div style={{width:"60%", margin: "1% 2% 1% 2%"}}>
                <WaveFileUpload />
            </div>
        </div>
    )
}
export default AudioVisualiserPage;