# Bailey Lind-Trefts    Spectralink

TRGT = sound_test

CC = gcc
CFLAGS = -g -Wall -Wl,-rpath=./objects/ 

FILES = main 

INCLUDE_DIR = /home/linuxbrew/.linuxbrew/Cellar/openal-soft/1.18.2/include/
INCLUDE_NAS_DIR = /home/nvidia/Downloads/openal-testing/nas-1.9.4/lib/audio/
INCLUDE_NAS_DIR__ = /home/nvidia/Downloads/openal-testing/nas0.9.4/lib/
INCLUDE_NAS_CONFIG_DIR = /home/nvidia/Downloads/openal-testing/nas-1.9.4/include/
INCLUDE_GLIB_DIR = /usr/include/glib-2.0/
INCLUDE_GLIBCONFIG_DIR = /usr/lib/aarch64-linux-gnu/glib-2.0/include/
INCLUDE_COMPAT_DIR = /usr/include/gstreamer-1.0/
INCLUDE_GSTARCH_DIR = /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include/
INCLUDE_AUDIOGST_DIR = /usr/include/gstreamer-1.0/gst/audio/

INCLUDES = $(patsubst %, $(INCLUDE_DIR)%.o, $(INCLUDE_FILES))

INCLUDE_DIR2 = /usr/include/audio/

SOURCE_DIR = src/

OBJ_DIR = objects/
OBJS = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))


$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ ./objects/wave.o ./objects/fileutil.o ./objects/libopenal.so.1.16.0 -lm #./objects/libaudio.so

$(OBJS): $(OBJ_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -I$(INCLUDE_COMPAT_DIR) -I$(INCLUDE_DIR) \
	-I$(INCLUDE_DIR2) -I$(INCLUDE_GLIBCONFIG_DIR) \
	-I$(INCLUDE_NAS_DIR) \
	-I$(INCLUDE_NAS_CONFIG_DIR) \
	-I$(INCLUDE_NAS_DIR__) \
	-I$(INCLUDE_AUDIOGST_DIR) -I$(INCLUDE_GSTARCH_DIR) -I$(INCLUDE_GLIB_DIR) $(CFLAGS) -o $@ -c $<

clean: 
	rm $(OBJS) $(TRGT)
