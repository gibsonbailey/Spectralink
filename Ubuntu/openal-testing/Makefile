# Bailey Lind-Trefts    Spectralink

TRGT = sound_test

CC = g++
CFLAGS = -g -Wall -Wl,-rpath=./objects/ -std=c++11

FILES = main 

INCLUDE_DIR = /usr/include/
INCLUDES = $(patsubst %, $(INCLUDE_DIR)%.o, $(INCLUDE_FILES))

INCLUDE_DIR2 = ./include/
INCLUDE_DIR3 = /usr/include/gstreamer-1.0/gst/
INCLUDE_DIR4 = /usr/include/gstreamer-1.0/
SOURCE_DIR = src/

OBJ_DIR = objects/
OBJS = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))


$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ ./objects/libopenal.so.1 ./objects/libaudio.so

$(OBJS): $(OBJ_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC)  -I$(INCLUDE_DIR) -I$(INCLUDE_DIR2) -I"/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include/gst" -I"/usr/lib/aarch64-linux-gnu/glib-2.0/include/" -I"/usr/include/glib-2.0" -I$(INCLUDE_DIR3) -I$(INCLUDE_DIR4) $(CFLAGS) -o $@ -c $<

clean: 
	rm $(OBJS) $(TRGT)
