INCLUDE = -IC:/MinGW/include/SDL/
LIBRARY = -LC:/MinGW/lib/
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDl2_mixer

all: chess.c chess_rules.c
	gcc chess.c chess_rules.c $(INCLUDE) $(LIBRARY) $(LINKER_FLAGS) -o chess