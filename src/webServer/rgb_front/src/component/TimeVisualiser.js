
import { useEffect} from 'react';
import './AudioVisualStyle.css';
import {setBoardWithScreen, createNewboard} from '../functions/board'

const TimeVisualiser = ({timeBoard, setTimeBoard, screen, canvasRef}) => {

    
    const canvasWidth = 800;
    const canvasHeight = 400;
    const maxBarHeight = 16;
    const pixelDiameter = canvasHeight / maxBarHeight;

    useEffect(() =>{
        let newBoard = [];
        let currentScreen = screen.value;
        newBoard = timeBoard;
        if(currentScreen){
            newBoard = setBoardWithScreen(newBoard, currentScreen)
        }
        setTimeBoard(newBoard);
    },[screen])

    useEffect(() => {
        if(timeBoard){
            drawCanvas();
        }
    },[screen])


    const drawCanvas = () =>{
        // draw the Time Visualiser
        // In pixels

        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

        ctx.fillStyle = `rgb(${0}, ${0}, ${0})`
        ctx.rect(0, 0, canvas.width, canvas.height);

        ctx.fill();

        for (let column = 0; column < 32; column++) {
            for (let row = 0; row < 16; row++) {
                ctx.beginPath();
                ctx.arc(
                    (pixelDiameter / 2) + (timeBoard[column][row].x * pixelDiameter),
                    (pixelDiameter / 2) + (timeBoard[column][row].y * pixelDiameter),
                    pixelDiameter / 2,
                    0,
                    2 * Math.PI
                );

                if(timeBoard[column][row].active){
                    ctx.fillStyle = timeBoard[column][row].color;
                }
                else{
                    ctx.fillStyle = `rgb(${50}, ${50}, ${50})`;
                }
                
                ctx.fill(); 
            }
        }
    }

    return (
        <div>
            <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/>
        </div>
    );
}

export default TimeVisualiser;
