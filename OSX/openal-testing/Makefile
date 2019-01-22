# Bailey Lind-Trefts    Spectralink
# Bailey Lind-Trefts    CS270
# HW4              10/18/2018

TRGT = sound_test

CC = gcc 
CFLAGS = -g -Wall 

FILES = main 

INCLUDE_DIR = /usr/local/opt/openal-soft/include/
INCLUDES = $(patsubst %, $(INCLUDE_DIR)%.o, $(INCLUDE_FILES))

SOURCE_DIR = src/

OBJ_DIR = objects/
OBJS = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))


$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ ./objects/libopenal.1.18.2.dylib ./objects/libalut.dylib

$(OBJS): $(OBJ_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -I$(INCLUDE_DIR) $(CFLAGS) -o $@ -c $<

clean: 
	rm $(OBJS) $(TRGT)
