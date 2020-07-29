#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "chess_rules.h"

typedef int boolean;
#define true 1
#define false 0

#define INIT_SCREEN_HEIGHT 480
#define INIT_SCREEN_WIDTH 640

/***********************************************************/
// 0 means white's turn, 1 means black's turn.
static int turn = 0;
// 0 means nobody has won yet, 1 means white has won, 2 means black has won, 3 means the game is a draw.
static int victor = 0;

// For rendering pieces and the chessboard
static int screen_width = INIT_SCREEN_WIDTH;
static int screen_height = INIT_SCREEN_HEIGHT;
static int square_size = 50;
static int width_window_spacing, height_window_spacing;

// 0 means no move, 1 means source has been chosen, 2 means destination has been chosen
static int move_state = 0; 
static SDL_Point move_source = {-1};
static SDL_Point move_dest = {-1};
static int chosen_piece;
/***********************************************************/

// Returns boolean representing whether or not initialization was successful.
extern boolean initialize();

// Render a piece given a struct representing the piece.
extern void renderPiece(Piece piece);

// Renders the chessboard
extern void renderChessboard();

// Renders all textures/images/shapes/etc. to the window.
extern void render();

// Continuously updates the size of the objects in the window using an event watcher
extern int resizeEventWatcher(void* data, SDL_Event* event);

// Adjusts the spacing variables so everything looks centered
extern void updateSpacing();

// Loads a texture to be used.
extern SDL_Texture* loadTextures(char texture_path[]);

// Loads music
extern boolean loadMusic(char music_path[]);

// Handles events (e.g: keyboard presses, mouse clicks, etc.)
extern void handleEvent();

// Move handler
extern void moveHandler();

// Steps through all processes not handled yet
extern void step();

// Shutdowns SDL smoothly.
extern boolean cleanup();

// Plays the current song
extern void playCurrent();

// Plays the next song in the playlist
extern void playlistNext();

// Plays the last song in the playlist
extern void playlistLast();

// SDL structures/unions
static Mix_Music *music = NULL;
static SDL_Window* window = NULL;
static SDL_Event event;
static SDL_Renderer* renderer = NULL;

static boolean running = false;
static char * texture_paths[] = {
    "Textures/white_pawn.png",
    "Textures/black_pawn.png",
    "Textures/white_knight.png",
    "Textures/black_knight.png",
    "Textures/white_bishop.png",
    "Textures/black_bishop.png",
    "Textures/white_rook.png",
    "Textures/black_rook.png",
    "Textures/white_queen.png",
    "Textures/black_queen.png",
    "Textures/white_king.png",
    "Textures/black_king.png",
};
static SDL_Texture *textures[sizeof(texture_paths) / sizeof(texture_paths[0])];

static int playlistCounter = 0;
static char * playlist[] = {
    "Music/Ballade_No_1.ogg",
    "Music/Nocturne_E_flat.ogg",
    "Music/Prelude_Book_2_mv5.wav",
    "Music/Arabesque.ogg",
    "Music/Moonlight_Sonata_2.ogg",
    "Music/Pathetique_2.ogg",
    "Music/Reverie.ogg",
    "Music/The_Girl_with_the_Flaxen_Hair.ogg",
    "Music/Piano_Sonata_No._11.ogg",
    "Music/Moonlight_Sonata_2.ogg"
};