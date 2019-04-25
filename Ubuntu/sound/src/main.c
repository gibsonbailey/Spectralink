#include "../include/wave.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static inline ALenum to_al_format(short channels, short samples);
int load_buffers(WaveInfo ** wave, ALuint * buffer);


int main(int argc, char * argv[])
{
    clock_t start_time = clock();
    clock_t load_start = clock();
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



    const int BUFFER_QUANTITY = 20;
    ALuint * buffer = (ALuint *)malloc(sizeof(ALuint) * BUFFER_QUANTITY);
    alGenBuffers( (ALuint)BUFFER_QUANTITY, buffer);
    // check for errors
    WaveInfo ** wave = (WaveInfo **)malloc(sizeof(WaveInfo *) * 20);
    load_buffers(wave, buffer);




    int num_srcs = argc / 4;
    printf("Number of sound sources: %d\n", num_srcs);
    ALuint * source = (ALuint *)malloc(sizeof(ALuint) * num_srcs);
    alGenSources((ALuint)num_srcs, source);

    int arg_count = 0;     
    int source_count = 0;     
    while(arg_count < argc)
    {
        unsigned char buffer_id = argv[arg_count][0] - 'a';
        char * x_location = argv[arg_count + 1];
        char * y_location = argv[arg_count + 2];
        char * z_location = argv[arg_count + 3];
    
    
        
        alSourcef(source[source_count], AL_PITCH, 1.0 / (1.1 - ((float)source_count / (float)num_srcs)));
        // check for errors
        alSourcef(source[source_count], AL_GAIN, 1);
        // check for errors
        
        float x_src = (float)atof(x_location);
        float y_src = (float)atof(y_location);
        float z_src = (float)atof(z_location);

        float dist = (float)sqrt((double)((x_src - x_lst) * (x_src - x_lst) + ((z_src - z_lst) * (z_src - z_lst)) + ((y_src - y_lst) * (y_src - y_lst))));
        x_src /= dist;
        y_src /= dist;
        z_src /= dist;

        alSource3f(source[source_count], AL_POSITION, x_src, y_src, z_src);
        // check for errors
        alSource3f(source[source_count], AL_VELOCITY, 0, 0, 0);
        // check for errors
        alSourcei(source[source_count], AL_LOOPING, AL_FALSE);
        // check for errros
        alSourcei(source[source_count], AL_BUFFER, buffer[buffer_id]);

        source_count++;
        arg_count += 4;
    }
    clock_t load_finish = clock();
        
    double load_dt = ((double)(load_finish - load_start)) / CLOCKS_PER_SEC;
    printf("Load time: %f\n", load_dt);

    clock_t play_start = clock();
    alSourcePlayv(num_srcs, source);
    clock_t play_current = clock();
    double play_dt = ((double)(play_current - play_start)) / CLOCKS_PER_SEC;
    printf("alSourcePlayv time: %f\n", play_dt);

    play_start = clock();
    play_current = clock();

    while(play_dt < 1.0)
    {
        play_current = clock();
        play_dt = ((double)(play_current - play_start)) / CLOCKS_PER_SEC;
    }
//    alSourceStopv(num_srcs, source);

    int source_state = AL_PLAYING;

    play_start = clock();
    while(source_state == AL_PLAYING)
    {
        alGetSourcei(source[0], AL_SOURCE_STATE, &source_state);
    }
    play_current = clock();
    play_dt = ((double)(play_current - play_start)) / CLOCKS_PER_SEC;
    printf("while playing time: %f\n", play_dt);

    printf("%s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alDeleteSources(num_srcs, source);
    
    alDeleteBuffers(BUFFER_QUANTITY, buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);


            clock_t current_time = clock();
        
            double dt = ((double)(current_time - start_time)) / CLOCKS_PER_SEC;
            printf("Main time: %f\n", (float)dt);
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


int load_buffers(WaveInfo ** wave, ALuint * buffer)
{
	char * saw261 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sawtooth261.wav";   //"a"
	char * saw349 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sawtooth349.wav";    //"b"
	char * saw493 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sawtooth493.wav";    //"c"
	char * saw659 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sawtooth659.wav";    //"d"
	char * saw880 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sawtooth880.wav";    //"e"
   char * sine261 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sine261.wav";        //"f"
   char * sine349 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sine349.wav";        //"g"
   char * sine493 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sine493.wav";        //"h"
   char * sine659 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sine659.wav";        //"i"
   char * sine880 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/sine880.wav";        //"j"
	char * squ261 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/square261.wav";      //"k"
	char * squ349 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/square349.wav";      //"l"
	char * squ493 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/square493.wav";      //"m"
	char * squ659 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/square659.wav";      //"n"
	char * squ880 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/square880.wav";      //"o"
	char * tri261 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/triangle261.wav";    //"p"
	char * tri349 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/triangle349.wav";    //"q"
	char * tri493 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/triangle493.wav";    //"r"
	char * tri659 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/triangle659.wav";    //"s"
	char * tri880 = "/usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/sound/data/triangle880.wav";    //"t"

    char ** filename = malloc(sizeof(char *) * 20);

    filename[0]  = saw261; 
    filename[1]  = saw349; 
    filename[2]  = saw493; 
    filename[3]  = saw659; 
    filename[4]  = saw880; 
    filename[5] = sine261;
    filename[6] = sine349;
    filename[7] = sine493;
    filename[8] = sine659;
    filename[9] = sine880;
    filename[10] = squ261; 
    filename[11] = squ349; 
    filename[12] = squ493; 
    filename[13] = squ659; 
    filename[14] = squ880; 
    filename[15] = tri261; 
    filename[16] = tri349; 
    filename[17] = tri493; 
    filename[18] = tri659; 
    filename[19] = tri880; 

    int ret;
    char * bufferData;

    for(int i = 0; i < 20; i++)
    {
        wave[i] = WaveOpenFileForReading(filename[i]);//sin_1000Hz_-10dBFS_0.03s.wav");//background_8bit.wav
        if (!wave[i]) {
                fprintf(stderr, "failed to read wave file\n");
                return -1;
        }

        ret = WaveSeekFile(0, wave[i]);
        if (ret) {
                fprintf(stderr, "failed to seek wave file\n");
                return -1;
        }

        bufferData = (char *)malloc(wave[i]->dataSize);
        if (!bufferData) {
                perror("malloc");
                return -1;
        }

        ret = WaveReadFile(bufferData, wave[i]->dataSize, wave[i]);
        if (ret != wave[i]->dataSize) {
                fprintf(stderr, "short read: %d, want: %d\n", ret, wave[i]->dataSize);
                return -1;
        }

        alBufferData(buffer[i], to_al_format(wave[i]->channels, wave[i]->bitsPerSample), bufferData, wave[i]->dataSize, wave[i]->sampleRate);
        free(bufferData);
    }

    free(filename);
    return 0;
}
