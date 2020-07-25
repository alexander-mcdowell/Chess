#include <stdio.h>
#include <stdlib.h>
#include "chess_rules.h"

/* TODO:
- Detect check/checkmate
- Promotion
- En Passant
*/

Piece pieces[32];
int board[8][8];
int castling = 0;

int getPiece(int row, int col) { return board[row - 1][col - 1]; }

boolean isLegal(Piece piece, int row, int col) {
    int i = getPiece(row, col);
    switch (piece.rank) {
        case PAWN:
            if (piece.color == WHITE && getPiece(piece.row - 1, piece.col) == -1) {
                // Move up two if haven't already moved
                if (piece.moved == false && piece.row == row + 2 && piece.col == col) return true;
                // Move up one
                if (piece.row == row + 1 && piece.col == col) return true;
            }
            else if (piece.color == BLACK && getPiece(piece.row + 1, piece.col) == -1) {
                // Move down two if haven't already moved
                if (piece.moved == false && piece.row == row - 2 && piece.col == col) return true;
                // Move down one
                if (piece.row == row - 1 && piece.col == col) return true;
            }
            break;
        case KNIGHT:
            if (i == -1 || pieces[i].color != piece.color) {
                // L-shaped moves
                if (abs(piece.row - row) == 1 && abs(piece.col - col) == 2) return true;
                if (abs(piece.row - row) == 2 && abs(piece.col - col) == 1) return true;
            }
            break;
        case BISHOP:
            // Line with slope 1 starting at (col, row) is y = x + row - col
            if (piece.col + row - col == piece.row) {
                // Check top left diagonal.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    int c = col - row + r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r -= 1;
                        c = col - row + r;
                    }
                }
                // Check bottom right diagonal.
                else {
                    int r = piece.row + 1;
                    int c = col - row + r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r += 1;
                        c = col - row + r;
                    }
                }
                if (i == -1 || pieces[i].color != piece.color) return true;
            // Line with slope -1 starting at (col, row) is y = -x + row + col
            } else if (-piece.col + row + col == piece.row) {
                // Check top right diagonal.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    int c = row + col - r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r -= 1;
                        c = row + col - r;
                    }
                }
                // Check bottom left diagonal.
                else {
                    int r = piece.row + 1;
                    int c = row + col - r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r += 1;
                        c = row + col - r;
                    }
                }
                if (i == -1 || pieces[i].color != piece.color) return true;
            }
            break;
        case ROOK:
            if (piece.row == row) {
                // Check to the left.
                if (piece.col > col) {
                    int c = piece.col - 1;
                    while (c != col) {
                        if (getPiece(piece.row, c) != - 1) return false;
                        c -= 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
                // Check to the right.
                else {
                    int c = piece.col + 1;
                    while (c != col) {
                        if (getPiece(piece.row, c) != - 1) return false;
                        c += 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
            } else if (piece.col == col) {
                // Check above.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    while (r != row) {
                        if (getPiece(r, piece.col) != - 1) return false;
                        r -= 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                } 
                // Check below.
                else {
                    int r = piece.row + 1;
                    while (r != row) {
                        if (getPiece(r, piece.col) != - 1) return false;
                        r += 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
            }
            break;
        case QUEEN:
            // Combines move availabilities from Queen and Rook
            if (piece.row == row) {
                // Check to the left.
                if (piece.col > col) {
                    int c = piece.col - 1;
                    while (c != col) {
                        if (getPiece(piece.row, c) != - 1) return false;
                        c -= 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
                // Check to the right.
                else {
                    int c = piece.col + 1;
                    while (c != col) {
                        if (getPiece(piece.row, c) != - 1) return false;
                        c += 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
            } else if (piece.col == col) {
                // Check above.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    while (r != row) {
                        if (getPiece(r, piece.col) != - 1) return false;
                        r -= 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                } 
                // Check below.
                else {
                    int r = piece.row + 1;
                    while (r != row) {
                        if (getPiece(r, piece.col) != - 1) return false;
                        r += 1;
                    }
                    if (i == -1 || pieces[i].color != piece.color) return true;
                }
            } else if (piece.col + row - col == piece.row) {
                // Check top left diagonal.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    int c = col - row + r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r -= 1;
                        c = col - row + r;
                    }
                }
                // Check bottom right diagonal.
                else {
                    int r = piece.row + 1;
                    int c = col - row + r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r += 1;
                        c = col - row + r;
                    }
                }
                if (i == -1 || pieces[i].color != piece.color) return true;
            } else if (-piece.col + row + col == piece.row) {
                // Check top right diagonal.
                if (piece.row > row) {
                    int r = piece.row - 1;
                    int c = row + col - r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r -= 1;
                        c = row + col - r;
                    }
                }
                // Check bottom left diagonal.
                else {
                    int r = piece.row + 1;
                    int c = row + col - r;
                    while (r != row && c != col) {
                        if (getPiece(r, c) != - 1) return false;
                        r += 1;
                        c = row + col - r;
                    }
                }
                if (i == -1 || pieces[i].color != piece.color) return true;
            }
            break;
        case KING:
            // Castling
            if (!piece.moved) {
                if (piece.color == WHITE) {
                    if (row == 8 && col == 3) {
                        i = getPiece(8, 1);
                        if (i != -1) {
                            if (pieces[i].rank == ROOK && pieces[i].color == piece.color && !pieces[i].moved) {
                                int count = 0;
                                for (int j = 2; j < 5; j++) {
                                    if (getPiece(8, j) == -1) count++;
                                }
                                if (count == 3) return true;
                            }
                        }
                    }
                    if (row == 8 && col == 7) {
                        i = getPiece(8, 8);
                        if (i != -1) {
                            if (pieces[i].rank == ROOK && pieces[i].color == piece.color && !pieces[i].moved) {
                                int count = 0;
                                for (int j = 6; j < 8; j++) {
                                    if (getPiece(8, j) == -1) count++;
                                }
                                if (count == 2) return true;
                            }
                        }
                    }
                } else {
                    if (row == 1 && col == 3) {
                        i = getPiece(1, 1);
                        if (i != -1) {
                            if (pieces[i].rank == ROOK && pieces[i].color == piece.color && !pieces[i].moved) {
                                int count = 0;
                                for (int j = 2; j < 5; j++) {
                                    if (getPiece(1, j) == -1) count++;
                                }
                                if (count == 3) return true;
                            }
                        }
                    }
                    if (row == 1 && col == 7) {
                        i = getPiece(1, 8);
                        if (i != -1) {
                            if (pieces[i].rank == ROOK && pieces[i].color == piece.color && !pieces[i].moved) {
                                int count = 0;
                                for (int j = 6; j < 8; j++) {
                                    if (getPiece(1, j) == -1) count++;
                                }
                                if (count == 2) return true;
                            }
                        }
                    }
                }
            }

            // Can move only one in every direction IF the move does not cause check
            if (abs(piece.row - row) <= 1 && abs(piece.col - col) <= 1) {
                if (!wouldBeCheck(piece, row, col) && (i == -1 || pieces[i].color != piece.color)) return true;
            }
            break;
    }
    return false;
}

boolean wouldBeCheck(Piece piece, int row, int col) {
    int i;

    // Check all squares to the right. If it intersects with one of the listed pieces, the move is illegal.
    int c = col - 1;
    while (c > 0) {
        i = getPiece(row, c);
        if (i != -1 && c != piece.col) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == ROOK || pieces[i].rank == QUEEN || (pieces[i].rank == KING && abs(c - col) == 1)))  return true;
            else break;
        }
        c -= 1;
    }
    // Check all squares to the left. If it intersects with one of the listed pieces, the move is illegal.
    c = col + 1;
    while (c < 9) {
        i = getPiece(row, c);
        if (i != -1 && c != piece.col) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == ROOK || pieces[i].rank == QUEEN || (pieces[i].rank == KING && abs(c - col) == 1)))  return true;
            else break;
        }
        c += 1;
    }

    // Check all squares above. If it intersects with one of the listed pieces, the move is illegal.
    int r = row - 1;
    while (r > 0) {
        i = getPiece(r, col);
        if (i != -1 && r != piece.row) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == ROOK || pieces[i].rank == QUEEN || (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r -= 1;
    }
    // Check all squares below. If it intersects with one of the listed pieces, the move is illegal.
    r = row + 1;
    while (r < 9 && r != piece.row) {
        i = getPiece(r, col);
        if (i != -1) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == ROOK || pieces[i].rank == QUEEN || (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r += 1;
    }
    
    // Check all squares to the top left. If it intersects with one of the listed pieces, the move is illegal.
    r = row - 1;
    c = col - 1;
    while (r > 0 && c > 0 && !(r == piece.row && c == piece.col)) {
        i = getPiece(r, c);
        if (i != -1) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == BISHOP || pieces[i].rank == QUEEN || \
                (pieces[i].rank == PAWN && abs(r - row) == 1 && pieces[i].color == BLACK) || \
                (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r -= 1;
        c = col - row + r;
    }
    // Check all squares to the top right. If it intersects with one of the listed pieces, the move is illegal.
    r = row - 1;
    c = col + 1;
    while (r > 0 && c < 9 && !(r == piece.row && c == piece.col)) {
        i = getPiece(r, c);
        if (i != -1) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == BISHOP || pieces[i].rank == QUEEN || \
                (pieces[i].rank == PAWN && abs(r - row) == 1 && pieces[i].color == BLACK) || \
                (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r -= 1;
        c = row + col - r;
    }
    // Check all squares to the bottom left. If it intersects with one of the listed pieces, the move is illegal.
    r = row + 1;
    c = col - 1;
    while (r < 9 && c > 0 && !(r == piece.row && c == piece.col)) {
        i = getPiece(r, c);
        if (i != -1) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == BISHOP || pieces[i].rank == QUEEN || \
                (pieces[i].rank == PAWN && abs(r - row) == 1 && pieces[i].color == WHITE) || \
                (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r += 1;
        c = row + col - r;
    }
     // Check all squares to the bottom right. If it intersects with one of the listed pieces, the move is illegal.
    r = row + 1;
    c = col + 1;
    while (r < 9 && c < 9 && !(r == piece.row && c == piece.col)) {
        i = getPiece(r, c);
        if (i != -1) {
            if (pieces[i].color != piece.color && \
                (pieces[i].rank == BISHOP || pieces[i].rank == QUEEN || \
                (pieces[i].rank == PAWN && abs(r - row) == 1 && pieces[i].color == WHITE) || \
                (pieces[i].rank == KING && abs(r - row) == 1))) return true;
            else break;
        }
        r += 1;
        c = col - row + r;
    }

    // Check knight moves
    if (row <= 7 && col <= 6) {
        i = getPiece(row + 1, col + 2);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row >= 1 && col <= 6) {
        i = getPiece(row - 1, col + 2);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row <= 7 && col >= 3) {
        i = getPiece(row + 1, col - 2);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row >= 1 && col >= 3) {
        i = getPiece(row - 1, col - 2);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row <= 6 && col <= 7) {
        i = getPiece(row + 2, col + 1);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row <= 6 && col >= 2) {
        i = getPiece(row + 2, col - 1);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row >= 3 && col <= 7) {
        i = getPiece(row - 2, col + 1);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }
    if (row >= 3 && col >= 2) {
        i = getPiece(row - 2, col - 1);
        if (i != -1) {
            if (pieces[i].rank == KNIGHT && pieces[i].color != piece.color) return true;
        }
    }

    // All possible moves have been checked, therefore the move is legal.
    return false;
}

void setupChessBoard() {
    Piece* ptr;

    // Setup black rooks
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, ROOK, 1, 1, false, false};
    pieces[0] = *ptr;
    board[0][0] = 0;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, ROOK, 1, 8, false, false};
    pieces[7] = *ptr;
    board[0][7] = 7;

    // Setup black knights
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, KNIGHT, 1, 2, false, false};
    pieces[1] = *ptr;
    board[0][1] = 1;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, KNIGHT, 1, 7, false, false};
    pieces[6] = *ptr;
    board[0][6] = 6;

    // Setup black bishops
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, BISHOP, 1, 3, false, false};
    pieces[2] = *ptr;
    board[0][2] = 2;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, BISHOP, 1, 6, false, false};
    pieces[5] = *ptr;
    board[0][5] = 5;

    // Setup black king and queen
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, QUEEN, 1, 4, false, false};
    pieces[3] = *ptr;
    board[0][3] = 3;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {BLACK, KING, 1, 5, false, false};
    pieces[4] = *ptr;
    board[0][4] = 4;

    // Setup black pawns
    for (int i = 8; i < 16; i++) {
        ptr = malloc(sizeof(Piece));
        *ptr = (Piece) {BLACK, PAWN, 2, i - 7, false, false};
        pieces[i] = *ptr;
        board[1][i - 8] = i;
    }

    // Setup white rooks
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, ROOK, 8, 1, false, false};
    pieces[16] = *ptr;
    board[7][0] = 16;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, ROOK, 8, 8, false, false};
    pieces[23] = *ptr;
    board[7][7] = 23;

    // Setup white knights
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, KNIGHT, 8, 2, false, false};
    pieces[17] = *ptr;
    board[7][1] = 17;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, KNIGHT, 8, 7, false, false};
    pieces[22] = *ptr;
    board[7][6] = 22;

    // Setup white bishops
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, BISHOP, 8, 3, false, false};
    pieces[18] = *ptr;
    board[7][2] = 18;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, BISHOP, 8, 6, false, false};
    pieces[21] = *ptr;
    board[7][5] = 21;

    // Setup white king and queen
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, QUEEN, 8, 4, false, false};
    pieces[19] = *ptr;
    board[7][3] = 19;
    ptr = malloc(sizeof(Piece));
    *ptr = (Piece) {WHITE, KING, 8, 5, false, false};
    pieces[20] = *ptr;
    board[7][4] = 20;

    // Setup white pawns
    for (int i = 24; i < 32; i++) {
        ptr = malloc(sizeof(Piece));
        *ptr = (Piece) {WHITE, PAWN, 7, i - 23, false, false};
        pieces[i] = *ptr;
        board[6][i - 24] = i;
    }
    
    // Fill in the rest of the board with empty values
    for (int row = 3; row < 7; row++) {
        for (int col = 1; col < 9; col++) board[row - 1][col - 1] = -1;
    }
}

int isCheck() {
    return 0;
}