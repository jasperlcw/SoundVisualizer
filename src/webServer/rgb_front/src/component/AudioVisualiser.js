
import { useEffect} from 'react';
import {setBoardWithSpectrum} from '../functions/board'

const AudioVisualiser = ({spectrum, board, setBoard, canvasRef}) => {

    
    // In pixels
    const maxBarHeight = 16;


    useEffect(() =>{
        let newBoard;
        // console.log(spectrum)
        const spectrumData = spectrum.data;
        newBoard = board;
        // no spectrum received
        if(spectrumData){
            newBoard = setBoardWithSpectrum(newBoard, spectrumData.value, spectrumData.color);
        }
        setBoard(newBoard);
        drawCanvasWithAudioSpectrum();
    },[spectrum])



    const drawCanvasWithAudioSpectrum = () =>{
        // draw the Audio Visualiser with bars
        // In pixels

        if(Object.keys(board).length === 0) return;

        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        ctx.fillStyle = `rgb(${0}, ${0}, ${0})`
        ctx.rect(0, 0, canvas.width, canvas.height);
        const pixelDiameter = canvas.height / maxBarHeight;
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
        </div>
    );
}

export default AudioVisualiser;
