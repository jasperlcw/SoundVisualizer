
import { useEffect, useRef} from 'react';
import './AudioVisualStyle.css';

const canvasHeight = 400;
const canvasWidth = 800;

const barWidth = canvasWidth / 16;
const barSpacing = 5;
const maxBarHeight = canvasHeight / 1.5;


const AudioVisualiser = ({spectrum}) => {
    // draw the Audio Visualiser with bars

    const canvasRef = useRef(null);
    useEffect(() =>{
        const visualData = [];
        const spectrumValues = spectrum.value;
        if(!spectrumValues){
            return;
        }
        for (let i = 0; i < spectrumValues.length && i < 16; i++) {
            const barHeight = spectrumValues[i] / 10 * maxBarHeight;
            const x = i * (barWidth + barSpacing);
            const y = canvasHeight - barHeight;

            visualData.push({
                x: x,
                y: y,
                width: barWidth,
                height: barHeight,
                color: `rgb(${255}, ${0}, ${0})`
            });
        }
        const canvas = canvasRef.current;
        const ctx = canvas.getContext('2d');

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        visualData.forEach(bar => {
            ctx.fillStyle = bar.color;
            ctx.fillRect(bar.x, bar.y, bar.width, bar.height);
        });

    },[spectrum])

    return (
        <div>
            <canvas ref = {canvasRef} id="audioSpectrum" className="audioCanvas" width={canvasWidth} height={canvasHeight}/>
        </div>
    );
}
export default AudioVisualiser;
