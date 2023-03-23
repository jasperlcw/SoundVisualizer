
import { useEffect, useRef} from 'react';
import './AudioVisualStyle.css';

// In pixels
const canvasHeight = 400;
const canvasWidth = 800;

const maxBarHeight = 16;
const pixelDiameter = canvasHeight / maxBarHeight;

// Initialize our board
let board = [];
for (let column = 0; column < 32; column++) {
    var row_array = [];
    for (let row = 0; row < 16; row++) {
        row_array.push({
            'x': column,
            'y': row,
            'active': false,
            'color': `rgb(${255}, ${0}, ${0})`
        })
    }
    board.push(row_array);
}

const AudioVisualiser = ({spectrum}) => {
    // draw the Audio Visualiser with bars

    const canvasRef = useRef(null);
    useEffect(() =>{
        const spectrumValues = spectrum.value;
        if(!spectrumValues){
            return;
        }

        for (let i = 0; i < spectrumValues.length && i < 32; i ++) {
            const barHeight = Math.floor(spectrumValues[i] / 10 * maxBarHeight);
            let currentIndex = 0;
            for (let row = maxBarHeight-1; row >= 0; row--) {
                if(currentIndex < barHeight) {
                    board[i][row].active = true;
                    // board[i + 1][row].active = true;
                } else {
                    board[i][row].active = false;
                    // board[i + 1][row].active = false;
                }
                currentIndex++;
            }
        }

        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

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

    },[spectrum])

    return (
        <div>
            <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/>
        </div>
    );
}

export default AudioVisualiser;
