#include <stdio.h>
#include "BeatController.h"

int beatSetting;
static int bpm;

wavedata_t presetSound[MAX_BEAT_SET];
wavedata_t uploadSound;

static bool isRunning;

static pthread_t beating;
void startBeatController(){
    isRunning = true;
    pthread_create(&beating, NULL, &startBeat, NULL);
}

void clearBeatController(){
    isRunning = false;
    pthread_join(beating, NULL);
}

void* startBeat(){
    
    runCommand("config-pin p9_18 i2c");
    runCommand("config-pin p9_17 i2c");

    // // Load wave file we want to play:
    // wavedata_t testFile;
    // AudioMixer_readWaveFileIntoMemory(FREQ_TESTER, &testFile);

    beatSetting = 0;

    // presetSound[0] = testFile;
    // presetSound[1] = drum_bd_hard;
    // presetSound[2] = drum_cc;
    // presetSound[3] = drum_snare_soft;

    while (isRunning)
    {
        if(beatSetting == 0){
            sleepForMs(1000);
        }
        else if(beatSetting == 1){
            playASound(0);
            sleepForMs(10000);
        }
    }
    return NULL;
}
//void setNewSound(){
//    wavedata_t testFile;
//}

void initialUploadWave(){

    if(uploadSound.pData != NULL){
        AudioMixer_freeWaveFileData(&uploadSound);
    }
    AudioMixer_readWaveFileIntoMemory(UPLOADED_WAVE_FILE, &uploadSound);
    AudioMixer_queueSound(&uploadSound);
}

bool playASound(int index){
    if (index >= 0 && index < MAX_SOUND_SET){
        AudioMixer_queueSound(&presetSound[index]);
        return true;
    }
    return false;
}

bool setBPM(int newBpm){
    if(MIN_BPM > newBpm || MAX_BPM < newBpm){
        //use for get value without prompt
        if(newBpm != -1000){
            printf("ERROR: BPM must be between %d and %d.\n", MIN_BPM, MAX_BPM);
        }
        return false;
    }
    bpm = newBpm;

    printf("current bpm: %d \n", bpm);
    return true;
}

int getBPM(){
    return bpm;
}

int getBPMms(double beat){
    double min = 60.0;
    return (int)(min / ((double)bpm) * 1000 * beat);
}

void switchBeatSetting(){
    beatSetting += 1;
    if(beatSetting >= MAX_BEAT_SET){
        beatSetting = 0;
    }
    printf("current beat: %d \n", beatSetting);
}

bool setBeatSetting(int setting){
    if (setting >=0 && setting < MAX_BEAT_SET){
        beatSetting = setting;
        return true;
    }
    return false;
}

int getBeatSetting(){
    return beatSetting;
}


