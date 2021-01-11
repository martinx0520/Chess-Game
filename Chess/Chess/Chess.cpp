#include <iostream>
#include <SFML/Graphics.hpp>

//Pieces
#define LEN 8
#define BPawn 1
#define BRook 2
#define BKnight 3
#define BBishop 4
#define BQueen 5
#define BKing 6
#define WPawn -1
#define WRook -2
#define WKnight -3
#define WBishop -4
#define WQueen -5
#define WKing -6

using namespace sf;

//Structure to replace pairs
struct pos {
    int x, y;
} oldpos, whiteKing, blackKing, transWhite, transBlack; // previous position, position of black and white kings and the promotion pawns

// The board
int board[8][8] =
{
    2, 3, 4, 5, 6, 4, 3, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -2, -3, -4, -5, -6, -4, -3, -2,
};
int rWhiteRook, lWhiteRook, WhiteKingFirstMove, rBlackRook, lBlackRook, BlackKingFirstMove; // Castle Vars
int transformationWhite, transformationBlack; // promotion status vars
int factor = 100, move = 0, isMoving, checkWhite, checkBlack,blackenpassant = 0, whiteenpassant = 0;//game status vars

// Piece Movement
int PawnW(int ox, int oy, int nx, int ny) {
    //move to blank spaces
    if (oldpos.y == 6) {
        if (ny == oy - 1 && nx == ox && board[oy - 1][ox] == 0) {
            return 1;
        }
        if (ny == oy - 2 && nx == ox && board[oy - 2][ox] == 0) {
            if (board[oy][ox - 1] == BPawn || board[oy][ox + 1] == BPawn) {
                blackenpassant = 1;
            }
            return 1;
        }
    }
    else if (ny == oy - 1 && nx == ox && board[oy - 1][ox] == 0) {
        return 1;
    }
    // taking black pieces
    if (board[oy - 1][ox - 1] > 0) {
        if (ny == oy - 1 && nx == ox - 1) {
            return 1;
        }
    }
    if (board[oy - 1][ox + 1] > 0) {
        if (ny == oy - 1 && nx == ox + 1) {
            return 1;
        }
    }
    //enpassant
    if (whiteenpassant == 1 && board[oy - 1][ox - 1] == 0) {
        if (ny == oy - 1 && nx == ox - 1) {
            whiteenpassant = 0;
            return 1;
        }
    }
    if (whiteenpassant == 1 && board[oy - 1][ox + 1] == 0) {
        if (ny == oy - 1 && nx == ox + 1) {
            whiteenpassant = 0;
            return 1;
        }
    }
    return 0;
}
int PawnB(int ox, int oy, int nx, int ny) {
    //move to blank spaces
    if (oldpos.y == 1) {
        if (ny == oy + 1 && nx == ox && board[oy + 1][ox] == 0) {
            return 1;
        }
        if (ny == oy + 2 && nx == ox && board[oy + 2][ox] == 0) {
            if (board[oy][ox - 1] == WPawn || board[oy][ox + 1] == WPawn) {
                whiteenpassant = 1;
            }
            return 1;
        }
    }
    else if (ny == oy + 1 && nx == ox && board[oy + 1][ox] == 0) {
        return 1;
    }
    //taking white pieces
    if (board[oy + 1][ox - 1] < 0) {
        if (ny == oy + 1 && nx == ox - 1) {
            return 1;
        }
    }
    if (board[oy + 1][ox + 1] < 0) {
        if (ny == oy + 1 && nx == ox + 1) {
            return 1;
        }
    }
    //enpassant
    if (blackenpassant == 1 && board[oy + 1][ox - 1] == 0) {
        if (ny == oy + 1 && nx == ox - 1) {
            blackenpassant = 0;
            return 1;
        }
    }
    if (blackenpassant == 1 && board[oy + 1][ox + 1] == 0) {
        if (ny == oy + 1 && nx == ox + 1) {
            blackenpassant = 0;
            return 1;
        }
    }
    return 0;
}
int RookW(int ox, int oy, int nx, int ny) {
    //left
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] >= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //up
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] >= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    //right
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] >= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //down
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] >= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    return 0;
}
int RookB(int ox, int oy, int nx, int ny) {
    //left
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] <= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //up
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] <= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    //right
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] <= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //down
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] <= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    return 0;
}
int BishopW(int ox, int oy, int nx, int ny) {
    //topleft
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //topright
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    //bottomleft
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //bottomright
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int BishopB(int ox, int oy, int nx, int ny) {
    //topleft
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //topright
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    //bottomleft
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //bottomright
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int QueenW(int ox, int oy, int nx, int ny) {
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] >= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] >= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] >= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] >= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int QueenB(int ox, int oy, int nx, int ny) {
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] <= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] <= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] <= 0 && (nx == i && ny == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] <= 0 && (ny == i && nx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (ny == i && nx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int KnightW(int ox, int oy, int nx, int ny) {
    //8 possible movements
    if (oy - 2 >= 0 && ox - 1 >= 0 && ny == oy - 2 && nx == ox - 1 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy - 2 >= 0 && ox + 1 < LEN && ny == oy - 2 && nx == ox + 1 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox - 2 >= 0 && ny == oy - 1 && nx == ox - 2 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 2 < LEN && ny == oy - 1 && nx == ox + 2 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox - 1 >= 0 && ny == oy + 2 && nx == ox - 1 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox + 1 < LEN && ny == oy + 2 && nx == ox + 1 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 2 >= 0 && ny == oy + 1 && nx == ox - 2 && board[ny][nx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 2 < LEN && ny == oy + 1 && nx == ox + 2 && board[ny][nx] >= 0) {
        return 1;
    }
    return 0;
}
int KnightB(int ox, int oy, int nx, int ny) {
    //8 possible movements
    if (oy - 2 >= 0 && ox - 1 >= 0 && ny == oy - 2 && nx == ox - 1 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy - 2 >= 0 && ox + 1 < LEN && ny == oy - 2 && nx == ox + 1 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox - 2 >= 0 && ny == oy - 1 && nx == ox - 2 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 2 < LEN && ny == oy - 1 && nx == ox + 2 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox - 1 >= 0 && ny == oy + 2 && nx == ox - 1 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox + 1 < LEN && ny == oy + 2 && nx == ox + 1 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 2 >= 0 && ny == oy + 1 && nx == ox - 2 && board[ny][nx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 2 < LEN && ny == oy + 1 && nx == ox + 2 && board[ny][nx] <= 0) {
        return 1;
    }
    return 0;
}
// checks
int PawnWCheck(int ox, int oy, int kingx, int kingy) {
    if (board[oy - 1][ox - 1] >= 0) {
        if (oy - 1 == kingy && ox - 1 == kingx) {
            return 1;
        }
    }
    if (board[oy - 1][ox + 1] >= 0) {
        if (oy - 1 == kingy && ox + 1 == kingx) {
            return 1;
        }
    }
    return 0;
}
int RookWCheck(int ox, int oy, int kingx, int kingy) {
    //leftcheck
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] >= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //upcheck
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] >= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    //rightcheck
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] >= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //downcheck
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] >= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    return 0;
}
int BishopWCheck(int ox, int oy, int kingx, int kingy) {
    //topleftcheck
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //toprightcheck
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    //bottomleftcheck
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //bottomrightcheck
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int QueenWCheck(int ox, int oy, int kingx, int kingy) {
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] >= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] >= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] >= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] >= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] >= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int KnightWCheck(int ox, int oy, int kingx, int kingy) {
    //8 possible checks
    if (oy - 2 >= 0 && ox - 1 >= 0 && kingy == oy - 2 && kingx == ox - 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy - 2 >= 0 && ox + 1 < LEN && kingy == oy - 2 && kingx == ox + 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox - 2 >= 0 && kingy == oy - 1 && kingx == ox - 2 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 2 < LEN && kingy == oy - 1 && kingx == ox + 2 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox - 1 >= 0 && kingy == oy + 2 && kingx == ox - 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox + 1 < LEN && kingy == oy + 2 && kingx == ox + 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 2 >= 0 && kingy == oy + 1 && kingx == ox - 2 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 2 < LEN && kingy == oy + 1 && kingx == ox + 2 && board[kingy][kingx] >= 0) {
        return 1;
    }
    return 0;
}
int KingWCheck(int ox, int oy, int kingx, int kingy) {
    //8 possible checks
    if (oy - 1 >= 0 && ox - 1 >= 0 && kingy == oy - 1 && kingx == ox - 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && kingy == oy - 1 && kingx == ox && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 1 < LEN && kingy == oy - 1 && kingx == ox + 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (ox + 1 < LEN && kingy == oy && kingx == ox + 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 1 < LEN && kingy == oy + 1 && kingx == ox + 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && kingy == oy + 1 && kingx == ox && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 1 >= 0 && kingy == oy + 1 && kingx == ox - 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    if (ox - 1 >= 0 && kingy == oy && kingx == ox - 1 && board[kingy][kingx] >= 0) {
        return 1;
    }
    return 0;
}
int PawnBCheck(int ox, int oy, int kingx, int kingy) {
    if (board[oy + 1][ox - 1] <= 0) {
        if (oy + 1 == kingy && ox - 1 == kingx) {
            return 1;
        }
    }
    if (board[oy + 1][ox + 1] <= 0) {
        if (oy + 1 == kingy && ox + 1 == kingx) {
            return 1;
        }
    }
    return 0;
}
int RookBCheck(int ox, int oy, int kingx, int kingy) {
    //leftcheck
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] <= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //upcheck
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] <= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    //rightcheck
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] <= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    //bottomcheck
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] <= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    return 0;
}
int BishopBCheck(int ox, int oy, int kingx, int kingy) {
    //topleftcheck
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //toprightcheck
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    //bottomleftcheck
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    //bottomrightcheck
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int QueenBCheck(int ox, int oy, int kingx, int kingy) {
    for (int i = ox - 1; i >= 0; i--) {
        if (board[oy][i] <= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][ox] <= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    for (int i = ox + 1; i < LEN; i++) {
        if (board[oy][i] <= 0 && (kingx == i && kingy == oy)) {
            return 1;
        }
        else if (board[oy][i] != 0) {
            break;
        }
    }
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][ox] <= 0 && (kingy == i && kingx == ox)) {
            return 1;
        }
        else if (board[i][ox] != 0) {
            break;
        }
    }
    int j = ox - 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy - 1; i >= 0; i--) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    j = ox - 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j--;
    }
    j = ox + 1;
    for (int i = oy + 1; i < LEN; i++) {
        if (board[i][j] <= 0 && (kingy == i && kingx == j)) {
            return 1;
        }
        else if (board[i][j] != 0) {
            break;
        }
        j++;
    }
    return 0;
}
int KnightBCheck(int ox, int oy, int kingx, int kingy) {
    //8 possible checks
    if (oy - 2 >= 0 && ox - 1 >= 0 && kingy == oy - 2 && kingx == ox - 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy - 2 >= 0 && ox + 1 < LEN && kingy == oy - 2 && kingx == ox + 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox - 2 >= 0 && kingy == oy - 1 && kingx == ox - 2 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 2 < LEN && kingy == oy - 1 && kingx == ox + 2 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox - 1 >= 0 && kingy == oy + 2 && kingx == ox - 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 2 < LEN && ox + 1 < LEN && kingy == oy + 2 && kingx == ox + 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 2 >= 0 && kingy == oy + 1 && kingx == ox - 2 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 2 < LEN && kingy == oy + 1 && kingx == ox + 2 && board[kingy][kingx] <= 0) {
        return 1;
    }
    return 0;
}
int KingBCheck(int ox, int oy, int kingx, int kingy){
    //8 possible checks
    if (oy - 1 >= 0 && ox - 1 >= 0 && kingy == oy - 1 && kingx == ox - 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && kingy == oy - 1 && kingx == ox && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox + 1 < LEN && kingy == oy - 1 && kingx == ox + 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (ox + 1 < LEN && kingy == oy && kingx == ox + 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox + 1 < LEN && kingy == oy + 1 && kingx == ox + 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && kingy == oy + 1 && kingx == ox && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (oy + 1 < LEN && ox - 1 >= 0 && kingy == oy + 1 && kingx == ox - 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    if (ox - 1 >= 0 && kingy == oy && kingx == ox - 1 && board[kingy][kingx] <= 0) {
        return 1;
    }
    return 0;
}
//King checks and movements
int WhiteKingCheck(int posKingx, int posKingy) {
    int check = 0;
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            if (board[i][j] != 0) {
                if (board[i][j] == BPawn) {
                    check = PawnBCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == BRook) {
                    check = RookBCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == BKnight) {
                    check = KnightBCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == BBishop) {
                    check = BishopBCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == BQueen) {
                    check = QueenBCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == BKing) {
                    check = KingBCheck(j, i, posKingx, posKingy);
                }
                if (check == 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
int BlackKingCheck(int posKingx, int posKingy) {
    int check = 0;
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            if (board[i][j] <= 0) {
                if (board[i][j] == WPawn) {
                    check = PawnWCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == WRook) {
                    check = RookWCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == WKnight) {
                    check = KnightWCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == WBishop) {
                    check = BishopWCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == WQueen) {
                    check = QueenWCheck(j, i, posKingx, posKingy);
                }
                if (board[i][j] == WKing) {
                    check = KingWCheck(j, i, posKingx, posKingy);
                }
                if (check == 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
int KingW(int ox, int oy, int nx, int ny) {
    //8 possible movements
    int wkc;
    if (oy - 1 >= 0 && ox - 1 >= 0 && ny == oy - 1 && nx == ox - 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox - 1, oy - 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (oy - 1 >= 0 && ny == oy - 1 && nx == ox && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox, oy - 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (oy - 1 >= 0 && ox + 1 < LEN && ny == oy - 1 && nx == ox + 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox + 1, oy - 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (ox + 1 < LEN && ny == oy && nx == ox + 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox + 1, oy);
        if (wkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ox + 1 < LEN && ny == oy + 1 && nx == ox + 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox + 1, oy + 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ny == oy + 1 && nx == ox && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox, oy + 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ox - 1 >= 0 && ny == oy + 1 && nx == ox - 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox - 1, oy + 1);
        if (wkc == 1) {
            return 1;
        }
    }
    if (ox - 1 >= 0 && ny == oy && nx == ox - 1 && board[ny][nx] >= 0) {
        wkc = WhiteKingCheck(ox - 1, oy);
        if (wkc == 1) {
            return 1;
        }
    }
    //right castle
    if (rWhiteRook == 0 && WhiteKingFirstMove == 0 && board[7][5] == 0 && board[7][6] == 0 && ny == 7 && nx == 6) {
        wkc = WhiteKingCheck(4, 7);
        if (wkc == 1) {
            wkc = WhiteKingCheck(5, 7);
            if (wkc == 1) {
                wkc = WhiteKingCheck(6, 7);
                if (wkc == 1) {
                    WhiteKingFirstMove = 1;
                    rWhiteRook = 1;
                    board[7][7] = 0;
                    board[7][5] = WRook;
                    return 1;
                }
            }
        }
    }
    //left castle
    if (lWhiteRook == 0 && WhiteKingFirstMove == 0 && board[7][3] == 0 && board[7][2] == 0 && board[7][1] == 0 && ny == 7 && nx == 2) {
        wkc = WhiteKingCheck(4, 7);
        if (wkc == 1) {
            wkc = WhiteKingCheck(3, 7);
            if (wkc == 1) {
                wkc = WhiteKingCheck(2, 7);
                if (wkc == 1) {
                    wkc = WhiteKingCheck(1, 7);
                    if (wkc == 1) {
                        WhiteKingFirstMove = 1;
                        lWhiteRook = 1;
                        board[7][0] = 0;
                        board[7][3] = WRook;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int KingB(int ox, int oy, int nx, int ny) {
    //8 possible movements
    int bkc;
    if (oy - 1 >= 0 && ox - 1 >= 0 && ny == oy - 1 && nx == ox - 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox - 1, oy - 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (oy - 1 >= 0 && ny == oy - 1 && nx == ox && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox, oy - 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (oy - 1 >= 0 && ox + 1 < LEN && ny == oy - 1 && nx == ox + 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox + 1, oy - 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (ox + 1 < LEN && ny == oy && nx == ox + 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox + 1, oy);
        if (bkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ox + 1 < LEN && ny == oy + 1 && nx == ox + 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox + 1, oy + 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ny == oy + 1 && nx == ox && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox, oy + 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (oy + 1 < LEN && ox - 1 >= 0 && ny == oy + 1 && nx == ox - 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox - 1, oy + 1);
        if (bkc == 1) {
            return 1;
        }
    }
    if (ox - 1 >= 0 && ny == oy && nx == ox - 1 && board[ny][nx] <= 0) {
        bkc = BlackKingCheck(ox - 1, oy);
        if (bkc == 1) {
            return 1;
        }
    }
    //right castle
    if (rBlackRook == 0 && BlackKingFirstMove == 0 && board[0][5] == 0 && board[0][6] == 0 && ny == 0 && nx == 6) {
        bkc = BlackKingCheck(4, 0);
        if (bkc == 1) {
            bkc = BlackKingCheck(5, 0);
            if (bkc == 1) {
                bkc = BlackKingCheck(6, 0);
                if (bkc == 1) {
                    BlackKingFirstMove = 1;
                    rBlackRook = 1;
                    board[0][7] = 0;
                    board[0][5] = BRook;
                    return 1;
                }
            }
        }
    }
    //left castle
    if (lBlackRook == 0 && BlackKingFirstMove == 0 && board[0][3] == 0 && board[0][2] == 0 && board[0][1] == 0 && ny == 0 && nx == 2) {
        bkc = BlackKingCheck(4, 0);
        if (bkc == 1) {
            bkc = BlackKingCheck(3, 0);
            if (bkc == 1) {
                bkc = BlackKingCheck(2, 0);
                if (bkc == 1) {
                    bkc = BlackKingCheck(1, 0);
                    if (bkc == 1) {
                        BlackKingFirstMove = 1;
                        lBlackRook = 1;
                        board[0][0] = 0;
                        board[0][3] = BRook;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
void posWhiteKing() {
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            if (board[i][j] == WKing) {
                whiteKing.x = j;
                whiteKing.y = i;
                break;
            }
        }
    }
}
void posBlackKing() {
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            if (board[i][j] == BKing) {
                blackKing.x = j;
                blackKing.y = i;
                break;
            }
        }
    }
}

int main()
{
    RenderWindow window(VideoMode(800, 800), "Chess");
    Texture p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15;

    p1.loadFromFile("Images/Board.png");
    p2.loadFromFile("Images/BlackPawn.png");
    p3.loadFromFile("Images/BlackRook.png");
    p4.loadFromFile("Images/BlackKnight.png");
    p5.loadFromFile("Images/BlackBishop.png");
    p6.loadFromFile("Images/BlackQueen.png");
    p7.loadFromFile("Images/BlackKing.png");
    p8.loadFromFile("Images/WhitePawn.png");
    p9.loadFromFile("Images/WhiteRook.png");
    p10.loadFromFile("Images/WhiteKnight.png");
    p11.loadFromFile("Images/WhiteBishop.png");
    p12.loadFromFile("Images/WhiteQueen.png");
    p13.loadFromFile("Images/WhiteKing.png");
    p14.loadFromFile("Images/transformationBlack.png");
    p15.loadFromFile("Images/transformationWhite.png");

    Sprite tiles(p1);
    Sprite BlackPawn(p2);
    Sprite BlackRook(p3);
    Sprite BlackKnight(p4);
    Sprite BlackBishop(p5);
    Sprite BlackQueen(p6);
    Sprite BlackKing(p7);
    Sprite WhitePawn(p8);
    Sprite WhiteRook(p9);
    Sprite WhiteKnight(p10);
    Sprite WhiteBishop(p11);
    Sprite WhiteQueen(p12);
    Sprite WhiteKing(p13);
    Sprite BlackTransformation(p14);
    Sprite WhiteTransformation(p15);
    Sprite Imageshift;

    float dx = 0, dy = 0;
    int unmoved = 0;
    Event e;
    while (window.isOpen()) {
        Vector2i pos = Mouse::getPosition(window);
        int x = pos.x / factor;
        int y = pos.y / factor;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
            window.clear();
            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    if (transformationWhite == 1) {
                        if (pos.y >= transWhite.y * factor && pos.y <= (transWhite.y + 1) * factor && pos.x >= transWhite.x * factor && pos.y <= (transWhite.x + 1) * factor) {
                            int xx = pos.x % factor, yy = pos.y % factor;
                            if (xx < 50 && yy < 50 && xx > 0 && yy > 0) {
                                board[transWhite.y][transWhite.x] = WBishop;
                                transformationWhite = 0;
                            }
                            if (xx > 50 && xx < 100 && yy > 50 && yy < 100) {
                                board[transWhite.y][transWhite.x] = WRook;
                                transformationWhite = 0;
                            }
                            if (xx < 50 && xx > 0 && yy > 50 && yy < 100) {
                                board[transWhite.y][transWhite.x] = WQueen;
                                transformationWhite = 0;
                            }
                            if (xx > 50 && xx < 100 && yy > 0 && yy < 50) {
                                board[transWhite.y][transWhite.x] = WKnight;
                                transformationWhite = 0;
                            }
                            if (transformationWhite == 0) {
                                posBlackKing();
                                int pbch = BlackKingCheck(blackKing.x, blackKing.y);
                                if (pbch == 0) {
                                    checkBlack = 1;
                                }
                            }
                        }
                    }
                    if (transformationBlack == 1) {
                        if (pos.y >= transBlack.y * factor && pos.y <= (transBlack.y + 1) * factor && pos.x >= transBlack.x * factor && pos.x <= (transBlack.x + 1) * factor) {
                            int xxx = pos.x % factor, yyy = pos.y % factor;
                            if (xxx < 50 && yyy < 50 && xxx > 0 && yyy > 0) {
                                board[transBlack.y][transBlack.x] = BBishop;
                                transformationBlack = 0;
                            }
                            if (xxx > 50 && xxx < 100 && yyy > 50 && yyy < 100) {
                                board[transBlack.y][transBlack.x] = BRook;
                                transformationBlack = 0;
                            }
                            if (xxx < 50 && xxx > 0 && yyy > 50 && yyy < 100) {
                                board[transBlack.y][transBlack.x] = BQueen;
                                transformationBlack = 0;
                            }
                            if (xxx > 50 && xxx < 100 && yyy > 0 && yyy < 50) {
                                board[transBlack.y][transBlack.x] = BKnight;
                                transformationBlack = 0;
                            }
                            if (transformationBlack == 0) {
                                posWhiteKing();
                                int pwch = WhiteKingCheck(whiteKing.x, whiteKing.y);
                                if (pwch == 0) {
                                    checkWhite = 1;
                                }
                            }
                        }
                    }
                    if (board[y][x] != 0) {
                        dx = pos.x - x * factor;
                        dy = pos.y - y * factor;
                        if (board[y][x] == BPawn && move == 1){
                            unmoved = BPawn;
                            Imageshift = BlackPawn;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == BRook && move == 1) {
                            unmoved = BRook;
                            Imageshift = BlackRook;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == BKnight && move == 1) {
                            unmoved = BKnight;
                            Imageshift = BlackKnight;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == BBishop && move == 1) {
                            unmoved = BBishop;
                            Imageshift = BlackBishop;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == BQueen && move == 1) {
                            unmoved = BQueen;
                            Imageshift = BlackQueen;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == BKing && move == 1) {
                            unmoved = BKing;
                            Imageshift = BlackKing;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == WPawn && move == 0) {
                            unmoved = WPawn;
                            Imageshift = WhitePawn;
                            board[y][x] = 0;
                        }
                        if (board[y][x] == WRook && move == 0) {
                            unmoved = WRook;
                            Imageshift = WhiteRook;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == WKnight && move == 0) {
                            unmoved = WKnight;
                            Imageshift = WhiteKnight;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == WBishop && move == 0) {
                            unmoved = WBishop;
                            Imageshift = WhiteBishop;
                            board[y][x] = 0;
                        }
                        if (board[y][x] == WQueen && move == 0) {
                            unmoved = WQueen;
                            Imageshift = WhiteQueen;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == WKing && move == 0) {
                            unmoved = WKing;
                            Imageshift = WhiteKing;
                            board[y][x] = 0;

                        }
                        if (board[y][x] == 0) {
                            isMoving = 1;
                            oldpos.x = x;
                            oldpos.y = y;
                        }
                    }
                }
            }
            if (e.type == Event::MouseButtonReleased) {
                if (e.key.code == Mouse::Left) {
                    int ok=2;
                    if (unmoved == WPawn && isMoving == 1) {
                        ok = PawnW(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == WRook && isMoving == 1) {
                        ok = RookW(oldpos.x, oldpos.y, x, y);
                        if (ok == 1 && lWhiteRook == 0 && oldpos.y == 7 && oldpos.x == 0) {
                            lWhiteRook = 1;
                        }
                        if (ok == 1 && rWhiteRook == 0 && oldpos.y == 7 && oldpos.x == 7) {
                            rWhiteRook = 1;
                        }
                    }
                    if (unmoved == WBishop && isMoving == 1) {
                        ok = BishopW(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == WQueen && isMoving == 1) {
                        ok = QueenW(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == WKnight && isMoving == 1) {
                        ok = KnightW(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == WKing && isMoving == 1) {
                        ok = KingW(oldpos.x, oldpos.y, x, y);
                        if (ok == 1 && WhiteKingFirstMove == 0) {
                            WhiteKingFirstMove = 1;
                        }
                    }
                    if (unmoved == BPawn && isMoving == 1) {
                        ok = PawnB(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == BRook && isMoving == 1) {
                        ok = RookB(oldpos.x, oldpos.y, x, y);
                        if (ok == 1 && lBlackRook == 0 && oldpos.y == 0 && oldpos.x == 0) {
                            lBlackRook = 1;
                        }
                        if (ok == 1 && rBlackRook == 0 && oldpos.y == 0 && oldpos.x == 7) {
                            rBlackRook = 1;
                        }
                    }
                    if (unmoved == BBishop && isMoving == 1) {
                        ok = BishopB(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == BQueen && isMoving == 1) {
                        ok = QueenB(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == BKnight && isMoving == 1) {
                        ok = KnightB(oldpos.x, oldpos.y, x, y);
                    }
                    if (unmoved == BKing && isMoving == 1) {
                        ok = KingB(oldpos.x, oldpos.y, x, y);
                        if (ok == 1 && BlackKingFirstMove == 0) {
                            BlackKingFirstMove = 1;
                        }
                    }
                    if (ok == 1) {
                        int nr = board[y][x];
                        board[y][x] = unmoved;
                        if (y == 0 && unmoved == WPawn) {
                            transformationWhite = 1;
                            transWhite.x = x;
                            transWhite.y = y;
                            board[y][x] = 0;
                        }
                        if (y == 7 && unmoved == BPawn) {
                            transformationBlack = 1;
                            transBlack.x = x;
                            transBlack.y = y;
                            board[y][x] = 0;
                        }
                        if (move == 0) {
                            if (checkWhite == 1) {
                                posWhiteKing();
                                int wch = WhiteKingCheck(whiteKing.x, whiteKing.y);
                                if (wch == 0) {
                                    board[oldpos.y][oldpos.x] = unmoved;
                                    board[y][x] = nr;
                                }
                                else {
                                    checkWhite = 0;
                                    posBlackKing();
                                    int bch = BlackKingCheck(blackKing.x, blackKing.y);
                                    if (bch == 0) {
                                        checkBlack = 1;
                                    }
                                    move = 1;
                                }
                            }
                            else {
                                posWhiteKing();
                                int wch = WhiteKingCheck(whiteKing.x, whiteKing.y);
                                if (wch == 0) {
                                    board[oldpos.y][oldpos.x] = unmoved;
                                    board[y][x] = nr;
                                }
                                else {
                                    posBlackKing();
                                    int bch = BlackKingCheck(blackKing.x, blackKing.y);
                                    if (bch == 0) {
                                        checkBlack = 1;
                                    }
                                    move = 1;
                                }
                            }
                        }
                        else {
                            if (checkBlack == 1) {
                                posBlackKing();
                                RectangleShape checksq(Vector2f(blackKing.x, blackKing.y));
                                checksq.setSize(Vector2f(100, 100));
                                checksq.setFillColor(Color(255, 0, 0));
                                int bch = BlackKingCheck(blackKing.x, blackKing.y);
                                if (bch == 0) {
                                    board[oldpos.y][oldpos.x] = unmoved;
                                    board[y][x] = nr;
                                }
                                else {
                                    checkBlack = 0;
                                    posWhiteKing();
                                    int wch = WhiteKingCheck(whiteKing.x, whiteKing.y);
                                    if (wch == 0) {
                                        checkWhite = 1;
                                    }
                                    move = 0;
                                }
                            }
                            else {
                                posBlackKing();
                                int bch = BlackKingCheck(blackKing.x, blackKing.y);
                                if (bch == 0) {
                                    board[oldpos.y][oldpos.x] = unmoved;
                                    board[y][x] = nr;
                                }
                                else {
                                    posWhiteKing();
                                    int wch = WhiteKingCheck(whiteKing.x, whiteKing.y);
                                    if (wch == 0) {
                                        checkWhite = 1;
                                    }
                                    move = 0;
                                }
                            }
                        }
                    }
                    else if (ok == 0) {
                        board[oldpos.y][oldpos.x] = unmoved;
                    }
                    isMoving = 0;
                    
                }
            }
        }
        window.clear();
        window.draw(tiles);
        if (transformationWhite == 1) {
            WhiteTransformation.setPosition(transWhite.x* factor, transWhite.y* factor);
            window.draw(WhiteTransformation);
        }
        if (transformationBlack == 1) {
            BlackTransformation.setPosition(transBlack.x* factor, transBlack.y* factor);
            window.draw(BlackTransformation);
        }
        if (checkWhite == 1) {
            RectangleShape wcheck(Vector2f(100, 100));
            wcheck.setPosition(whiteKing.x* factor, whiteKing.y* factor);
            wcheck.setFillColor(Color(255, 0, 0));
            window.draw(wcheck);
        }
        if (checkBlack == 1) {
            RectangleShape bcheck(Vector2f(100, 100));
            bcheck.setPosition(blackKing.x* factor, blackKing.y* factor);
            bcheck.setFillColor(Color(255, 0, 0));
            window.draw(bcheck);
        }
        if (isMoving == 1) {
            Imageshift.setPosition(pos.x - dx, pos.y - dy);
            window.draw(Imageshift);
        }
        if (blackenpassant == 1 && move == 0) {
            blackenpassant = 0;
        }
        if (whiteenpassant == 1 && move == 1) {
            whiteenpassant = 0;
        }
        for (int i = 0; i < LEN; i++) {
            for (int j = 0; j < LEN; j++) {
                if (board[i][j] != 0) {
                    if (board[i][j] == BPawn) {
                        BlackPawn.setPosition(j* factor, i* factor);
                        window.draw(BlackPawn);
                    }
                    if (board[i][j] == BRook) {
                        BlackRook.setPosition(j* factor, i* factor);
                        window.draw(BlackRook);
                    }
                    if (board[i][j] == BKnight) {
                        BlackKnight.setPosition(j* factor, i* factor);
                        window.draw(BlackKnight);
                    }
                    if (board[i][j] == BBishop) {
                        BlackBishop.setPosition(j* factor, i* factor);
                        window.draw(BlackBishop);
                    }
                    if (board[i][j] == BQueen) {
                        BlackQueen.setPosition(j* factor, i* factor);
                        window.draw(BlackQueen);
                    }
                    if (board[i][j] == BKing) {
                        BlackKing.setPosition(j* factor, i* factor);
                        window.draw(BlackKing);
                    }
                    if (board[i][j] == WPawn) {
                        WhitePawn.setPosition(j * factor, i * factor);
                        window.draw(WhitePawn);
                    }
                    if (board[i][j] == WRook) {
                        WhiteRook.setPosition(j * factor, i * factor);
                        window.draw(WhiteRook);
                    }
                    if (board[i][j] == WKnight) {
                        WhiteKnight.setPosition(j * factor, i * factor);
                        window.draw(WhiteKnight);
                    }
                    if (board[i][j] == WBishop) {
                        WhiteBishop.setPosition(j * factor, i * factor);
                        window.draw(WhiteBishop);
                    }
                    if (board[i][j] == WQueen) {
                        WhiteQueen.setPosition(j * factor, i * factor);
                        window.draw(WhiteQueen);
                    }
                    if (board[i][j] == WKing) {
                        WhiteKing.setPosition(j * factor, i * factor);
                        window.draw(WhiteKing);
                    }
                }
            }
        }
        window.display();


    }
   

    return 0;
}

