#include </usr/local/opt/openal-soft/include/AL/al.h>
#include </usr/local/opt/openal-soft/include/AL/alc.h>
#include <stdio.h>

int main()
{
    ALCdevice *device;

    device = alcOpenDevice(NULL);
    if (device == NULL)
    {
        
    }

    return 0;
}
