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

    var barColour;
    if (brightness < 12.5 * 1) {
        barColour = "#000000";
    } else if (brightness < 12.5 * 2) {
        barColour = "#ff0000";
    } else if (brightness < 12.5 * 3) {
        barColour = "#00ff00";
    } else if (brightness < 12.5 * 4) {
        barColour = "#ffff00";
    } else if (brightness < 12.5 * 5) {
        barColour = "#0000ff";
    } else if (brightness < 12.5 * 6) {
        barColour = "#ff00ff";
    } else if (brightness < 12.5 * 7) {
        barColour = "#00ffff";
    } else {
        barColour = "#ffffff";
    }

    return (
        <div id="brightness" style={{ width: '200px', backgroundColor: '#ddd'}} >
            <div id="brightnessBar" style={{ width: `${brightness}%`, height: '30px', backgroundColor: `${barColour}`}}></div>
        </div>
    );
}

export default BrightnessBar;