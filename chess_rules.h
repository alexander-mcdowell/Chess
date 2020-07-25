typedef int boolean;
#define true 1
#define false 0

enum Color {WHITE, BLACK};
enum Rank {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

struct Piece {
    enum Color color;
    enum Rank rank;
    int row;
    int col;
    boolean moved;
    boolean captured;
};
typedef struct Piece Piece;

extern Piece pieces[32];
extern int board[8][8];

// Gets the index of the piece located at a given row/col, returns -1 if no piece found
extern int getPiece(int row, int col);

// Checks if a location is a legal move for a piece
extern boolean isLegal(Piece piece, int row, int col);

// Returns whether a king who moves to a given row and column would be in check.
extern boolean wouldBeCheck(Piece piece, int row, int col);

// Sets the locations for each of the pawns, knights, etc.
extern void setupChessBoard();

// Checks to see if the current game setup is in check or checkmate. 
// 0 = no check, 1 = white check, 2 = black check, 3 = white checkmate, 4 = black checkmate
extern int isCheck();