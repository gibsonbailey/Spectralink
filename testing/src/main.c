#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>

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

    ALsizei size, freq;
    ALenum format;
    ALvoid *data;
    ALboolean loop = AL_FALSE;

    alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);







    printf("%s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

    alcCloseDevice(device);

    return 0;
}
