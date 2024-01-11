#pragma once
static const char* RULES = "\
Chesscade is a falling block puzzle game with chess pieces. Pieces fall\n\
from the top of the board. When a piece reaches the bottom, it captures\n\
all pieces it could capture in chess from its position. When a king is\n\
captured or the next piece can't spawn, the game ends. As a piece falls,\n\
a cursor moves across the top of the board. When it reaches one side, it\n\
wraps around to the other. When a capture is made, the cursor changes\n\
directions. When a piece lands, the next piece is determined by where\n\
the cursor is, whether the cursor wrapped around, and whether the last\n\
piece made any captures. If the cursor wrapped, a king spawns.\n\
Otherwise, if the last piece made any captures, a rook, knight, bishop,\n\
or queen spawns, depending on the position of the cursor. Otherwise, a\n\
pawn spawns. Each piece can only reach squares it could reach in chess.\n\
Thus, black pawns spawn on the rank 7, white pawns land on rank 2, and\n\
bishops move 2 squares at a time so they stay on their colors. White\n\
pawns capture upward and black pawns capture downward. If a white pawn\n\
lands on rank 8 or a black pawn lands on rank 1, it promotes to a queen\n\
before capturing. The current 'combo' is the number of consecutive\n\
turns on which pieces have been captured. The higher the combo, the\n\
faster pieces fall. No points can be scored if the board state is\n\
identical to any previous board state in the current game. The points\n\
scored by a given piece are equal to:\n\
[total value of pieces captured] x [number of pieces captured] x [combo]\n\
Piece values: pawn=1 knight=3 bishop=3 rook=5 queen=9 king=0\n\
";
#define RULES_LENGTH 1598