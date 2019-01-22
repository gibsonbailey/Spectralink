# Bailey Lind-Trefts    Spectralink

TRGT = sound_test

CC = gcc 
CFLAGS = -g -Wall -Wl,-rpath=./objects/

FILES = main 

INCLUDE_DIR = /home/linuxbrew/.linuxbrew/Cellar/openal-soft/1.18.2/include/
INCLUDES = $(patsubst %, $(INCLUDE_DIR)%.o, $(INCLUDE_FILES))

INCLUDE_DIR2 = ./includes/

SOURCE_DIR = src/

OBJ_DIR = objects/
OBJS = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))


$(TRGT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ ./objects/libopenal.so ./objects/libaudio.so

$(OBJS): $(OBJ_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -I$(INCLUDE_DIR) -I$(INCLUDE_DIR2) $(CFLAGS) -o $@ -c $<

clean: 
	rm $(OBJS) $(TRGT)
