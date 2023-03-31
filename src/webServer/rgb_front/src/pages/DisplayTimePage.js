import {useRef, useEffect} from 'react';

import './Page.css';

import TimeVisualiser from "../component/TimeVisualiser";

// functions
import {createNewboard} from '../functions/board';

// In pixels
const canvasHeight = 400;
const canvasWidth = 800;



const DisplayTimePage = ({board, setBoard, screen, sendMessage}) => {

    const canvasRef = useRef(null);

      //send getScreen to bbg
    useEffect(()=>{
        const Interval = setInterval(()=>{
        sendMessage("getScreen");
        }, 100)
        return () => clearInterval(Interval);
    },[])

    //initial board
    useEffect(()=>{
        const newBoard = createNewboard();
        setBoard(newBoard);
    }, [])

    return (
        <div className="page-header">
            <h1>TIME</h1>
            <div>
                <TimeVisualiser board = {board} setBoard = {setBoard} screen = {screen} canvasRef ={canvasRef}/>
            </div>
            <div>
                <canvas ref = {canvasRef} id="Time" className="TimeCanvas" width={canvasWidth} height={canvasHeight}/>
            </div>

        </div>
    )
}
export default DisplayTimePage;