import {useRef, useEffect} from 'react';

import './Page.css';

import TimeVisualiser from "../component/TimeVisualiser";

// functions
import {createNewboard} from '../functions/board';

// In pixels
const canvasHeight = 400;
const canvasWidth = 800;


const DisplayTimePage = ({timeBoard, setTimeBoard, screen, sendMessage, canvasRef}) => {
      //send getScreen to bbg
    useEffect(()=>{
        const Interval = setInterval(()=>{
        sendMessage("getScreen");
        }, 1000)
        return () => clearInterval(Interval);
    },[])

    //initial board
    useEffect(()=>{
        const newBoard = createNewboard();
        setTimeBoard(newBoard);
    }, [])

    return (
        <div className="page-header">
            {/* <h1>TIME</h1> */}
            <div>
                <TimeVisualiser timeBoard = {timeBoard} setTimeBoard = {setTimeBoard} screen = {screen} canvasRef ={canvasRef}/>
            </div>
            
            <div className = "audioContainer">
                <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={1000} height={500}/>
            </div>

        </div>
    )
}
export default DisplayTimePage;