import {useEffect, useState} from 'react';


const VolumeController = ({sendMessage, volume, setVolume}) => {
    //send getVolume when init

    const [volumeControl, setVolumeControl] = useState(volume);
    // useEffect(()=>{
    //     sendMessage("getVolume");
    //     setVolumeControl(volume);
    // },[]);

    //send getBrightness to bbg
    useEffect(()=>{
    const Interval = setInterval(()=>{
        sendMessage(`setVolume ${volumeControl}`);
    }, 500)
    return () => clearInterval(Interval);
    },[volumeControl]);

    // set a new volume
    const setNewVolume = (e) =>{
        setVolumeControl(e.target.value);
    }

    return (
        <div style={{width:"100%", height:"100%", transform: 'scale(2)'}}>
            <input type="range" min="0" max="100" value={volumeControl} onChange={setNewVolume} />
        </div>
    )
}
export default VolumeController;