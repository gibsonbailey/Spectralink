#include <AL/al.h>
#include <AL/alc.h>
#include <wave.h>
#include <stdio.h>
#include <stdlib.h>

static inline ALenum to_al_format(short channels, short samples);

int main()
{
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


    ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    // check for errors
    alListener3f(AL_VELOCITY, 0, 0, 0);
    // check for errors
    alListenerfv(AL_ORIENTATION, listenerOri);







    ALuint source;

    alGenSources( (ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1);
    // check for errors
    alSourcef(source, AL_GAIN, 1);
    // check for errors
    alSource3f(source, AL_POSITION, 0, 0, 0);
    // check for errors
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    // check for errors
    alSourcei(source, AL_LOOPING, AL_FALSE);
    // check for errros



    ALuint buffer;

    alGenBuffers( (ALuint)1, &buffer);
    // check for errors




    WaveInfo *wave;
    char *bufferData;
    int ret;
    
    wave = WaveOpenFileForReading("./data/sin_1000Hz_-10dBFS_0.03s.wav");
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



    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);

    int source_state;

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);

    while(source_state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    }

    for(int i = 0; i < 1000000; i++)
    {
        // Delay loop
    }

    printf("%s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

    // cleanup context
    alDeleteSources(1, &source);
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

