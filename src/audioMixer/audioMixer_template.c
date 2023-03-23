// Incomplete implementation of an audio mixer. Search for "REVISIT" to find things
// which are left as incomplete.
// Note: Generates low latency audio on BeagleBone Black; higher latency found on host.
#include "audioMixer_template.h"
#include <alsa/asoundlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <limits.h>
#include <alloca.h> // needed for mixer
#include <math.h>

// #include "../lib/fftw-3.5.8/api/fftw3.h"
#include <fftw3.h>
#include <stdio.h>

static snd_pcm_t *handle;

//fftw
static double *fftwIn;
static fftw_complex *fftwOut;
static fftw_plan fftwPlan;
int fftwCount;
static pthread_t spectrumThread;
double* spectrum;


static unsigned long playbackBufferSize = 0;
static short *playbackBuffer = NULL;

typedef struct {
    // A pointer to a previously allocated sound bite (wavedata_t struct).
    // Note that many different sound-bite slots could share the same pointer
    // (overlapping cymbal crashes, for example)
    wavedata_t *pSound;

    // The offset into the pData of pSound. Indicates how much of the
    // sound has already been played (and hence where to start playing next).
    int location;
} playbackSound_t;
static playbackSound_t soundBites[MAX_SOUND_BITES];

// Playback threading
void* playbackThread(void* arg);
static bool stopping = false;
static pthread_t playbackThreadId;
static pthread_mutex_t lock;

static int volume = 0;

void AudioMixer_init(void)
{
    AudioMixer_setVolume(DEFAULT_VOLUME);

    //init mutex
    pthread_mutex_init(&lock, NULL);

    // Initialize the currently active sound-bites being played
    // REVISIT:- Implement this. Hint: set the pSound pointer to NULL for each
    //     sound bite.

    for(int i=0; i< MAX_SOUND_BITES; i++){
        soundBites[i].pSound = NULL;
        soundBites[i].location = 0;
    }

    // Open the PCM output
    int err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    // Configure parameters of PCM output
    err = snd_pcm_set_params(handle,
                             SND_PCM_FORMAT_S16_LE,
                             SND_PCM_ACCESS_RW_INTERLEAVED,
                             NUM_CHANNELS,
                             SAMPLE_RATE,
                             1,			// Allow software resampling
                             50000);		// 0.05 seconds per buffer
    if (err < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    // Allocate this software's playback buffer to be the same size as the
    // the hardware's playback buffers for efficient data transfers.
    // ..get info on the hardware buffers:
    unsigned long unusedBufferSize = 0;
    snd_pcm_get_params(handle, &unusedBufferSize, &playbackBufferSize);
    // ..allocate playback buffer:
    playbackBuffer = malloc(playbackBufferSize * sizeof(*playbackBuffer));




    // Launch playback thread:
    pthread_create(&playbackThreadId, NULL, playbackThread, NULL);
    startSpectrumThread();
}


// Client code must call AudioMixer_freeWaveFileData to free dynamically allocated data.
void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound)
{
    assert(pSound);

    // The PCM data in a wave file starts after the header:
    const int PCM_DATA_OFFSET = 44;

    // Open the wave file
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s.\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    int sizeInBytes = ftell(file) - PCM_DATA_OFFSET;
    pSound->numSamples = sizeInBytes / SAMPLE_SIZE;

    // Search to the start of the data in the file
    fseek(file, PCM_DATA_OFFSET, SEEK_SET);

    // Allocate space to hold all PCM data
    pSound->pData = malloc(sizeInBytes);
    if (pSound->pData == 0) {
        fprintf(stderr, "ERROR: Unable to allocate %d bytes for file %s.\n",
                sizeInBytes, fileName);
        exit(EXIT_FAILURE);
    }

    // Read PCM data from wave file into memory
    int samplesRead = fread(pSound->pData, SAMPLE_SIZE, pSound->numSamples, file);
    if (samplesRead != pSound->numSamples) {
        fprintf(stderr, "ERROR: Unable to read %d samples from file %s (read %d).\n",
                pSound->numSamples, fileName, samplesRead);
        exit(EXIT_FAILURE);
    }
}

void AudioMixer_freeWaveFileData(wavedata_t *pSound)
{
    pSound->numSamples = 0;
    free(pSound->pData);
    pSound->pData = NULL;
}

void AudioMixer_queueSound(wavedata_t *pSound)
{
    // Ensure we are only being asked to play "good" sounds:
    assert(pSound->numSamples > 0);
    assert(pSound->pData);

    // Insert the sound by searching for an empty sound bite spot
    /*
     * REVISIT: Implement this:
     * 1. Since this may be called by other threads, and there is a thread
     *    processing the soundBites[] array, we must ensure access is threadsafe.
     * 2. Search through the soundBites[] array looking for a free slot.
     * 3. If a free slot is found, place the new sound file into that slot.
     *    Note: You are only copying a pointer, not the entire data of the wave file!
     * 4. After searching through all slots, if no free slot is found then print
     *    an error message to the console (and likely just return vs asserting/exiting
     *    because the application most likely doesn't want to crash just for
     *    not being able to play another wave file.
     */
    pthread_mutex_lock(&lock);

    int index = 0;
    while(soundBites[index].pSound != NULL){
        index ++;
        if(index >= MAX_SOUND_BITES){
            fprintf(stderr, "ERROR: no more space for queue sound.\n");
            pthread_mutex_unlock(&lock);
            return;
        }
    }
    soundBites[index].pSound = pSound;
    soundBites[index].location = 0;
    pthread_mutex_unlock(&lock);
}

void AudioMixer_cleanup(void)
{
    printf("Stopping audio...\n");

    // Stop the PCM generation thread
    stopping = true;
    pthread_join(playbackThreadId, NULL);

    clearSpectrumThread();

    // Shutdown the PCM output, allowing any pending sound to play out (drain)
    snd_pcm_drain(handle);
    snd_pcm_close(handle);

    // Free playback buffer
    // (note that any wave files read into wavedata_t records must be freed
    //  in addition to this by calling AudioMixer_freeWaveFileData() on that struct.)
    free(playbackBuffer);
    playbackBuffer = NULL;

    printf("Done stopping audio...\n");
    fflush(stdout);
}


int AudioMixer_getVolume()
{
    // Return the cached volume; good enough unless someone is changing
    // the volume through other means and the cached value is out of date.
    return volume;
}

// Function copied from:
// http://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code
// Written by user "trenki".
bool AudioMixer_setVolume(int newVolume)
{

    // Ensure volume is reasonable; If so, cache it for later getVolume() calls.
    if (newVolume < AUDIOMIXER_MIN_VOLUME || newVolume > AUDIOMIXER_MAX_VOLUME) {
        //use for get value without prompt
        if(newVolume != -1000){
            printf("ERROR: Volume must be between %d and %d.\n", AUDIOMIXER_MIN_VOLUME, AUDIOMIXER_MAX_VOLUME);
        }
        return false;
    }
    volume = newVolume;
    printf("current volume: %d \n", newVolume);

    long min, max;
    snd_mixer_t *volHandle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "PCM";

    snd_mixer_open(&volHandle, 0);
    snd_mixer_attach(volHandle, card);
    snd_mixer_selem_register(volHandle, NULL, NULL);
    snd_mixer_load(volHandle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(volHandle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(volHandle);
    return true;
}

// Fill the `buff` array with new PCM values to output.
//    `buff`: buffer to fill with new PCM data from sound bites.
//    `size`: the number of values to store into playbackBuffer
static void fillPlaybackBuffer(short *buff, int size)
{
    //Wipe the playbackBuffer to all 0's to clear any previous PCM data.
    memset(buff , 0, sizeof(*buff) * size);

    pthread_mutex_lock(&lock);
    for(int i=0; i<MAX_SOUND_BITES; i++){

        wavedata_t* tempPSound = soundBites[i].pSound;
        // If the sound bite slot is unused, do nothing for this slot.
        if(tempPSound == NULL) continue;

        int index = 0;
        int tempNSamples = tempPSound->numSamples;
        int offsetLocation = soundBites[i].location;

        while (index < size && offsetLocation < tempNSamples){
            int sample = buff[index] + tempPSound->pData[offsetLocation];

            //PCM is stored as signed shorts (between SHRT_MIN and SHRT_MAX).
            sample = sample < SHRT_MIN ? SHRT_MIN : sample;
            sample = sample > SHRT_MAX ? SHRT_MAX : sample;

            buff[index] = (short)sample;
            index++;
            offsetLocation++;
        }

        //If you have now played back the entire sample, free the slot in the soundBites[] array.
        if(offsetLocation >= tempPSound->numSamples){
            soundBites[i].pSound = NULL;
        }
        soundBites[i].location = offsetLocation;
    }
    pthread_mutex_unlock(&lock);
}

void* playbackThread(void* arg)
{

    while (isRunning) {
        // Generate next block of audio
        fillPlaybackBuffer(playbackBuffer, playbackBufferSize);


        // Output the audio
        snd_pcm_sframes_t frames = snd_pcm_writei(handle,
                                                  playbackBuffer, playbackBufferSize);

        // Check for (and handle) possible error conditions on output
        if (frames < 0) {
            fprintf(stderr, "AudioMixer: writei() returned %li\n", frames);
            frames = snd_pcm_recover(handle, frames, 1);
        }
        if (frames < 0) {
            fprintf(stderr, "ERROR: Failed writing audio with snd_pcm_writei(): %li\n",
                    frames);
            exit(EXIT_FAILURE);
        }
        if (frames > 0 && frames < playbackBufferSize) {
            printf("Short write (expected %li, wrote %li)\n",
                   playbackBufferSize, frames);
        }
    }

    return NULL;
}

// Spectrum
void startSpectrumThread(){
    //init fftw plans
    // "fftwCount / 2" will be the number of output for the Spectrum

    fftwCount = 32;
    fftwIn = (double*) fftw_malloc(sizeof (double) * playbackBufferSize);
    fftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) *playbackBufferSize);
    fftwPlan = fftw_plan_dft_r2c_1d(fftwCount, fftwIn, fftwOut, FFTW_ESTIMATE);

    spectrum = malloc(sizeof(double) * (fftwCount/2));
    pthread_create(&spectrumThread, NULL, &generateSpectrum, NULL);
}

void clearSpectrumThread(){
    pthread_join(spectrumThread, NULL);
    fftw_destroy_plan(fftwPlan);
}

double* getSpectrum(){
    return spectrum;
}

int getSpectrumCount(){
    return fftwCount / 2;
}

void* generateSpectrum(){
    //Spectrum range
    //example: https://nanohub.org/resources/16909/download/2013.02.08-ECE595E-L13.pdf
    while(isRunning){
        pthread_mutex_lock(&lock);

        //short to audio double
        for (int i = 0; i < playbackBufferSize; i++){
            fftwIn[i] = playbackBuffer[i] / 32768.0;
        }
        pthread_mutex_unlock(&lock);
        fftw_execute(fftwPlan);

        for(int i = 0; i < fftwCount / 2; i++){
            // fftw output is a complex number;
            double real = fftwOut[i][0];
            double img = fftwOut[i][1];
            spectrum[i] = sqrt(real * real + img * img);
        }

//        for(int i = 0; i < fftwCount / 2; i++){
//            printf("%d: %0.0f ", i, spectrum[i] * 1000);
//        }
//        printf("\n");
        sleepForMs(100);
    }
    return NULL;
}
