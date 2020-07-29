#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chess.h"

boolean initialize() {
    boolean initialize = true; 

    // Initialize Video and Audio
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        (void) printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        initialize = false;
    }
    else {
        // Create a window with inital screen width/height
        window = SDL_CreateWindow("Chess", 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  SDL_WINDOWPOS_UNDEFINED, 
                                  INIT_SCREEN_WIDTH, 
                                  INIT_SCREEN_HEIGHT, 
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            (void) printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
            initialize = false;
        } else {
            // Make the window resizable 
            SDL_SetWindowResizable(window, true);
            SDL_SetWindowMinimumSize(window, 8 * square_size, 8 * square_size);
            updateSpacing();

            // Create the renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL) {
                (void) printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError() );
                initialize = false;
            }
            else {
                // Initalize IMG Library
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    (void) printf("SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
                    initialize = false;
                }

                // Initalize Text library
                if (TTF_Init() == -1) {
                    (void) printf("SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError() );
                    initialize = false;
                }

                // Initialize Mixer library
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    (void) printf("SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
                    initialize = false;
                }
            }
        }
    }
    
    return initialize;
}

void renderPiece(Piece piece) {
    SDL_Rect* pos = malloc(sizeof(SDL_Rect));
    pos->x = width_window_spacing + square_size * (piece.col - 1);
    pos->y = height_window_spacing + square_size * (piece.row - 1);
    pos->w = square_size;
    pos->h = square_size;

    switch (piece.rank) {
        case PAWN:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[0], NULL, pos);
            else SDL_RenderCopy(renderer, textures[1], NULL, pos);
            break;
        case KNIGHT:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[2], NULL, pos);
            else SDL_RenderCopy(renderer, textures[3], NULL, pos);
            break;
        case BISHOP:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[4], NULL, pos);
            else SDL_RenderCopy(renderer, textures[5], NULL, pos);
            break;
        case ROOK:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[6], NULL, pos);
            else SDL_RenderCopy(renderer, textures[7], NULL, pos);
            break;
        case QUEEN:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[8], NULL, pos);
            else SDL_RenderCopy(renderer, textures[9], NULL, pos);
            break;
        case KING:
            if (piece.color == WHITE) SDL_RenderCopy(renderer, textures[10], NULL, pos);
            else SDL_RenderCopy(renderer, textures[11], NULL, pos);
            break;
    }
}

void renderChessboard() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    // Draw boundary lines
    for (int i = 0; i < 9; i++) {
        SDL_RenderDrawLine(renderer, width_window_spacing + square_size * i, height_window_spacing, width_window_spacing + square_size * i, height_window_spacing + square_size * 8);
        SDL_RenderDrawLine(renderer, width_window_spacing, height_window_spacing + square_size * i, width_window_spacing + square_size * 8, height_window_spacing + square_size * i);
    }

    // Draw squares
    SDL_Rect *rect;
    int row = 1;
    int col = 1;
    boolean dark = false;
    while (row != 9) {
        // If a piece is chosen, render the legal moves red and the chosen piece green, otherwise alternate tan/dark brown.
        if (move_state == 1) {
            if (move_source.x == col && move_source.y == row) SDL_SetRenderDrawColor(renderer, 100, 250, 65, SDL_ALPHA_OPAQUE);     // Light Green
            else {
                if (isLegal(pieces[chosen_piece], row, col)) {
                    SDL_SetRenderDrawColor(renderer, 255, 50, 50, SDL_ALPHA_OPAQUE);    // Light Red
                }
                else {
                    if (!dark) SDL_SetRenderDrawColor(renderer, 79, 6, 0, SDL_ALPHA_OPAQUE);   // Dark brown
                    else SDL_SetRenderDrawColor(renderer, 247, 203, 132, SDL_ALPHA_OPAQUE);    // Tan
                }
            }
        }
        else {
            // Alternate dark brown/tan
            if (!dark) SDL_SetRenderDrawColor(renderer, 79, 6, 0, SDL_ALPHA_OPAQUE);   // Dark brown
            else SDL_SetRenderDrawColor(renderer, 247, 203, 132, SDL_ALPHA_OPAQUE);    // Tan
        }
        rect = malloc(sizeof(SDL_Rect));
        *rect = (SDL_Rect) {width_window_spacing + square_size * (col - 1), height_window_spacing + square_size * (row - 1), square_size, square_size};
        SDL_RenderFillRect(renderer, rect);

        col += 1;
        if (col == 9) {
            col = 1;
            row += 1;
        }
        else dark = 1 - dark;
    }
}

void render() {
    // Draw the background white
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, NULL);
    
    // Render the chessboard THEN the pieces IF they are not captured.
    renderChessboard();
    for (int i = 0; i < 32; i++) {
        if (!pieces[i].captured) renderPiece(pieces[i]);
    }

    SDL_RenderPresent(renderer);
}

// Function called every time the window is resized. Makes resizing look smoother.
int resizeEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
        if (win == (SDL_Window*)data) {
            updateSpacing();
            render();
        }
    }
    return 0;
}

void updateSpacing() {
    SDL_GetWindowSize(window, &screen_width, &screen_height);
    if (screen_width < 8 * square_size) screen_width = 8 * square_size;
    if (screen_height < 8 * square_size) screen_height = 8 * square_size;
    width_window_spacing = (screen_width - 8 * square_size) / 2;
    height_window_spacing = (screen_height - 8 * square_size) / 2;
}

SDL_Texture* loadTexture(char texture_path[]) {
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(texture_path);

    if (loadedSurface == NULL) (void) printf("Unable to load image %s. SDL_image Error: %s\n", texture_path, IMG_GetError());
    else {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL) (void) printf("Unable to create texture from %s. SDL Error: %s\n", texture_path, SDL_GetError());

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return texture;
}

boolean loadMusic(char music_path[]) {
    music = Mix_LoadMUS(music_path);
    if (music == NULL) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    } else return true;
}

void handleEvent() {
    while (SDL_PollEvent(&event) != 0) {
        // X button
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            render();
        }

        int mouseX, mouseY;
        int row, col;
        SDL_GetMouseState(&mouseX, &mouseY);
        // Check if mouse is in the chessboard
        if ((width_window_spacing < mouseX && mouseX < width_window_spacing + square_size * 8) && 
            (height_window_spacing < mouseY && mouseY < height_window_spacing + square_size * 8)) {
            // Get row and col from mouse (x, y)
            row = 1 + (mouseY - height_window_spacing) / square_size;
            col = 1 + (mouseX - width_window_spacing) / square_size;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Selecting a piece to move
                if (move_state == 0) {
                    int x = getPiece(row, col);
                    if (x != -1) {
                        if (!pieces[x].captured && ((turn == 0 && pieces[x].color == WHITE) || (turn == 1 && pieces[x].color == BLACK))) {
                            chosen_piece = x;
                            move_source = (SDL_Point) {col, row};
                            move_state = 1;
                        }
                    }
                }
                // Moving a selected piece
                else if (move_state == 1) {
                    // Cancelling a move
                    if (row == move_source.y && col == move_source.x) {
                        move_source = (SDL_Point) {-1};
                        move_state = 0;
                    }
                    // Moving to a new location
                    else {
                        move_dest = (SDL_Point) {col, row};
                        move_state = 2;
                    }
                }
            }
        }

        // Playlist controls
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // Pause the song.
                case SDLK_p:
                    if (Mix_PausedMusic() == 0) Mix_PauseMusic();
                    else Mix_ResumeMusic();
                    break;
                // Restart the song.
                case SDLK_r:
                    Mix_HaltMusic();
                    Mix_PlayMusic(music, 1);
                    break;
                // Next song.
                case SDLK_RIGHT:
                    playlistNext();
                    break;
                // Previous song.
                case SDLK_LEFT:
                    playlistLast();
                    break;
                default:
                    break;
            }
        }
    }
}

void moveHandler() {
    int i;
    if (isLegal(pieces[chosen_piece], move_dest.y, move_dest.x)) {
        // Handle castling
        if (pieces[chosen_piece].rank == KING && !(abs(move_dest.y - move_source.y) == 1 || abs(move_source.x - move_dest.x) == 1)) {
            // White left rook
            if (move_dest.x == 3 && move_dest.y == 8) {
                i = getPiece(8, 1);
                pieces[i].row = 8;
                pieces[i].col = 4;
                pieces[chosen_piece].row = 8;
                pieces[chosen_piece].col = 3;
                board[7][0] = -1;
                board[7][4] = -1;
                board[7][3] = i;
                board[7][2] = chosen_piece;
            }
            // White right rook
            else if (move_dest.x == 7 && move_dest.y == 8) {
                i = getPiece(8, 8);
                pieces[i].row = 8;
                pieces[i].col = 6;
                pieces[chosen_piece].row = 8;
                pieces[chosen_piece].col = 7;
                board[7][7] = -1;
                board[7][4] = -1;
                board[7][5] = i;
                board[7][6] = chosen_piece;
            }
            // Black left rook
            else if (move_dest.x == 3 && move_dest.y == 1) {
                i = getPiece(1, 1);
                pieces[i].row = 1;
                pieces[i].col = 4;
                pieces[chosen_piece].row = 1;
                pieces[chosen_piece].col = 3;
                board[0][0] = -1;
                board[0][4] = -1;
                board[0][3] = i;
                board[0][2] = chosen_piece;
            }
            // Black right rook
            else if (move_dest.x == 7 && move_dest.y == 1) {
                i = getPiece(1, 8);
                pieces[i].row = 1;
                pieces[i].col = 6;
                pieces[chosen_piece].row = 1;
                pieces[chosen_piece].col = 7;
                board[0][7] = -1;
                board[0][4] = -1;
                board[0][5] = i;
                board[0][6] = chosen_piece;
            }
        }
        // En passant capture
        else if (pieces[chosen_piece].rank == PAWN && abs(move_source.x - move_dest.x) == 1 && abs(move_source.y - move_dest.y) == 1 \
                && pieces[getPiece(move_source.y, move_dest.x)].rank == PAWN) {
            i = getPiece(move_source.y, move_dest.x);
            pieces[i].captured = true;
            
            board[move_source.y - 1][move_source.x - 1] = -1;
            board[move_dest.y - 1][move_dest.x - 1] = chosen_piece;
            board[move_source.y - 1][move_dest.x - 1] = -1;

            pieces[chosen_piece].row = move_dest.y;
            pieces[chosen_piece].col = move_dest.x;
        }
        else {
            // Moving up two for pawns
            if (pieces[chosen_piece].rank == PAWN && abs(move_dest.y - move_source.y) == 2) {
                pieces[chosen_piece].moved = 2;
            }

            // Change the board location
            i = getPiece(move_dest.y, move_dest.x);
            board[move_source.y - 1][move_source.x - 1] = -1;
            board[move_dest.y - 1][move_dest.x - 1] = chosen_piece;

            // Capture the piece at destination
            if (i != -1) {
                pieces[i].captured = true;
            }

            // Change the piece location
            pieces[chosen_piece].row = move_dest.y;
            pieces[chosen_piece].col = move_dest.x;
            if (!pieces[chosen_piece].moved) pieces[chosen_piece].moved = true;
        }

        // Pawn promotion
        if (pieces[chosen_piece].rank == PAWN && \
            ((pieces[chosen_piece].rank == WHITE && pieces[chosen_piece].row == 1) || (pieces[chosen_piece].rank == BLACK && pieces[chosen_piece].row == 8))) {
            // Prompt user for the rank of the new piece
            int choice;
            while (true) {
                (void) printf("Desired piece for promotion? (Enter one of the numbers)\n 1. Knight.\n 2. Bishop.\n 3. Rook.\n 4. Queen.\n");
                (void) scanf("%d", &choice);
                if (choice >= 1 && choice <= 4) break;
            }
            switch (choice) {
                case 1:
                    pieces[chosen_piece].rank = KNIGHT;
                    break;
                case 2:
                    pieces[chosen_piece].rank = BISHOP;
                    break;
                case 3:
                    pieces[chosen_piece].rank = ROOK;
                    break;
                case 4:
                    pieces[chosen_piece].rank = QUEEN;
                    break;
            }
        }

        move_source = (SDL_Point) {-1};
        move_dest = (SDL_Point) {-1};
        move_state = 0;
        turn = 1 - turn;
    }
    // Move illegal, try again.
    else move_state = 1;
}

void step() {
    if (Mix_PlayingMusic() == 0) playCurrent();

    // Analyzing a chosen move
    if (move_state == 2) moveHandler();
}

boolean cleanup() {
    boolean cleaned = true;

    for (int i = 0; i < sizeof(textures) / sizeof(textures[0]); i++) {
        SDL_DestroyTexture(textures[i]);
        textures[i] = NULL;
    }

    for (int i = 0; i < 32; i++) free(&pieces[i]);

    Mix_FreeMusic(music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    music = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return cleaned;
}

void playCurrent() {
    if (loadMusic(playlist[playlistCounter]) == true) {
        loadMusic(playlist[playlistCounter]);
        Mix_PlayMusic(music, 1);
    }
}

void playlistNext() {
    playlistCounter += 1;
    if (playlistCounter >= sizeof(playlist) / sizeof(playlist[0])) playlistCounter = 0;
    Mix_HaltMusic();
    playCurrent();
}

void playlistLast() {
    playlistCounter -= 1;
    if (playlistCounter < 0) playlistCounter = sizeof(playlist) / sizeof(playlist[0]) - 1;
    Mix_HaltMusic();
    playCurrent();
}

int main(int argc, char *argv[]) {
    if (!initialize()) return 1;
    (void) printf("Initialization complete.\n");
    running = true;

    for (int i = 0; i < sizeof(texture_paths) / sizeof(texture_paths[0]); i++) {
        textures[i] = loadTexture(texture_paths[i]);
    }
    (void) printf("Textures loaded.\n");

    setupChessBoard();
    (void) printf("Chessboard setup.\n");

    SDL_AddEventWatch(resizeEventWatcher, window);
    
    while (running) {
        handleEvent();
        step();
        render();
        SDL_Delay(15);
    }

    if (!cleanup()) return 1;
    (void) printf("Cleanup successful.\n");
    return 0;
}
