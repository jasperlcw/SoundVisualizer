
import { useEffect} from 'react';
import './AudioVisualStyle.css';
import {setBoardWithSpectrum} from '../functions/board'

const AudioVisualiser = ({spectrum, board, setBoard, canvasRef}) => {

    
    // In pixels

    const canvasWidth = 800;
    const canvasHeight = 400;
    const maxBarHeight = 16;
    const pixelDiameter = canvasHeight / maxBarHeight;

    useEffect(() =>{
        let newBoard;
        const spectrumValues = spectrum.value;
        newBoard = board;
        // no spectrum received
        if(spectrumValues){
            newBoard = setBoardWithSpectrum(newBoard, spectrumValues);
        }
        setBoard(newBoard);
    },[spectrum])

    useEffect(() => {
        if(board){
            drawCanvasWithAudioSpectrum();
        }
    },[spectrum])


    const drawCanvasWithAudioSpectrum = () =>{
        // draw the Audio Visualiser with bars
        // In pixels

        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        ctx.fillStyle = `rgb(${0}, ${0}, ${0})`
        ctx.rect(0, 0, canvas.width, canvas.height);
        ctx.fill();

        for (let column = 0; column < 32; column++) {
            for (let row = 0; row < 16; row++) {
                ctx.beginPath();
                ctx.arc(
                    (pixelDiameter / 2) + (board[column][row].x * pixelDiameter),
                    (pixelDiameter / 2) + (board[column][row].y * pixelDiameter),
                    pixelDiameter / 2,
                    0,
                    2 * Math.PI
                );
                if(board[column][row].active)
                    ctx.fillStyle = board[column][row].color
                else
                    ctx.fillStyle = `rgb(${50}, ${50}, ${50})`;
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

export default AudioVisualiser;
