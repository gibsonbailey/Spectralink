#include "../wave.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static inline ALenum to_al_format(short channels, short samples);


int main(){
    ALCdevice *device;

    device = alcOpenDevice(NULL);
    if (device == NULL)
    {
        printf("Error initializing Audio Device.\n");        
    }


    ALCcontext *context;

    context = alcCreateContext(device, NULL);
    if(!alcMakeContextCurrent(context))
    {
        printf("Error creating context.\n");
    }

    float x_lst, y_lst, z_lst;
    x_lst = 0.0;
    y_lst = 0.0;
    z_lst = 0.0;

    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, x_lst, y_lst, z_lst);
    // check for errors
    alListener3f(AL_VELOCITY, 0, 0, 0);
    // check for errors
    alListenerfv(AL_ORIENTATION, listenerOri);


    ALuint buffer;
    alGenBuffers( (ALuint)1, &buffer);
    // check for errors


    WaveInfo *wave;
    char *bufferData;
    int ret;

    wave = WaveOpenFileForReading("./data/sin_1000Hz_-10dBFS_0.03s.wav");//sin_1000Hz_-10dBFS_0.03s.wav");//background_8bit.wav
    if (!wave) {
            fprintf(stderr, "failed to read wave file\n");
            return -1;
    }

    ret = WaveSeekFile(0, wave);
    if (ret) {
            fprintf(stderr, "failed to seek wave file\n");
            return -1;
    }

    bufferData = malloc(wave->dataSize);
    if (!bufferData) {
            perror("malloc");
            return -1;
    }

    ret = WaveReadFile(bufferData, wave->dataSize, wave);
    if (ret != wave->dataSize) {
            fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
            return -1;
    }

    alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample), bufferData, wave->dataSize, wave->sampleRate);

    int num_srcs = 10;
    double duration = 0.02;
    int repeats = 5;
    
    ALuint source[num_srcs];
    alGenSources( (ALuint)num_srcs, source);
    for(int i = 0; i < num_srcs; i++)
    { 
        alSourcef(source[i], AL_PITCH, 1.0 / (1.1 - ((float)i / (float)num_srcs)));
        // check for errors
        alSourcef(source[i], AL_GAIN, 1);
        // check for errors
        float pos_scale = 10.0;
        float pos = pos_scale * ((float)i / (float)(num_srcs - 1)) - (pos_scale / 2.0);
        float x_src, y_src, z_src;
        x_src = pos;
        y_src = 0.0; // y is the vertical dimension and has no bearing on the inter-aural time difference
        z_src = 1.0;

        float dist = (float)sqrt((double)((x_src - x_lst) * (x_src - x_lst) + ((z_src - z_lst) * (z_src - z_lst))));
        x_src /= dist;
        z_src /= dist;

        alSource3f(source[i], AL_POSITION, x_src, y_src, z_src);
        // check for errors
        alSource3f(source[i], AL_VELOCITY, 0, 0, 0);
        // check for errors
        alSourcei(source[i], AL_LOOPING, AL_FALSE);
        // check for errros
        alSourcei(source[i], AL_BUFFER, buffer);
    }


    for(int i = 0; i < num_srcs; i++)
    {
        for(int j = 0; j < repeats; j++)
        {
            alSourcePlay(source[i]);
        
            int source_state = AL_PLAYING;
            
            clock_t start_time = clock();
            clock_t current_time = clock();
        
            double dt = ((double)(current_time - start_time)) / CLOCKS_PER_SEC;
            while(dt < duration)
            {
                while(source_state == AL_PLAYING)
                {
                    alGetSourcei(source[i], AL_SOURCE_STATE, &source_state);
                }
                current_time = clock();
                dt = ((double)(current_time - start_time)) / CLOCKS_PER_SEC;
            }
        }
    }

    for(int i = 0; i < 1000000; i++)
    {
        // Delay loop
    }

    printf("%s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

    // cleanup context
    for(int i = 0; i < num_srcs; i++){
	    alDeleteSources(1, &source[i]);
    }
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);


    return 0;
}

static inline ALenum to_al_format(short channels, short samples)
{
        int stereo = (channels > 1);

        switch (samples) {
        case 16:
                if (stereo)
                        return AL_FORMAT_STEREO16;
                else
                        return AL_FORMAT_MONO16;
        case 8:
                if (stereo)
                        return AL_FORMAT_STEREO8;
                else
                        return AL_FORMAT_MONO8;
        default:
                return -1;
        }
}

