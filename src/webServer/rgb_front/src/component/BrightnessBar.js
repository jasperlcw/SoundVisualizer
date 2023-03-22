import { useEffect } from 'react';

// Modified from https://www.w3schools.com/howto/howto_js_progressbar.asp

const BrightnessBar = ({brightness}) => {
    useEffect(() =>{
        if(brightness > 100) {
            brightness = 100;
        } else if (brightness < 0) {
            brightness = 0;
        }
    },[brightness])

    return (
        <div id="brightness" style={{ width: '200px', backgroundColor: '#ddd'}} >
            <div id="brightnessBar" style={{ width: `${brightness}%`, height: '30px', backgroundColor: '#8b0000'}}></div>
        </div>
    );
}

export default BrightnessBar;