
import { useEffect} from 'react';
import './AudioVisualStyle.css';
import {setBoardWithSpectrum} from '../functions/board'

const AudioVisualiser = ({spectrum, board, setBoard, canvasRef}) => {

    let newBoard = [];
    const canvasHeight = 400;
    const maxBarHeight = 16;
    const pixelDiameter = canvasHeight / maxBarHeight;

    useEffect(() =>{
        const spectrumValues = spectrum.value;
        newBoard = board;
        // no spectrum received
        if(spectrumValues){
            newBoard = setBoardWithSpectrum(newBoard, spectrumValues);
        }
        drawCanvasWithAudioSpectrum();
        setBoard(newBoard);
    },[spectrum])


    const drawCanvasWithAudioSpectrum = () =>{
        // draw the Audio Visualiser with bars
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
                    (pixelDiameter / 2) + (newBoard[column][row].x * pixelDiameter),
                    (pixelDiameter / 2) + (newBoard[column][row].y * pixelDiameter),
                    pixelDiameter / 2,
                    0,
                    2 * Math.PI
                );
                if(newBoard[column][row].active)
                    ctx.fillStyle = newBoard[column][row].color
                else
                    ctx.fillStyle = `rgb(${50}, ${50}, ${50})`;
                ctx.fill(); 
            }
        }
    }

    return (
        <div>
            {/* <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/> */}
        </div>
    );
}

export default AudioVisualiser;
