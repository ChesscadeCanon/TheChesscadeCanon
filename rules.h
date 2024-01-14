#pragma once
static const char* RULES = "\
Chesscade is a falling block puzzle game with chess pieces. Pieces fall\n\
from the top of the board. When a piece reaches the bottom, it captures\n\
all pieces it could capture in chess from its position. When a king is\n\
captured or the next piece can't spawn, the game ends. As a piece falls,\n\
a cursor moves across the top of the board. When it reaches one side, it\n\
wraps around to the other. When a capture is made, the cursor changes\n\
directions. When a piece lands, the next piece is determined by where\n\
the cursor is, whether the cursor wrapped around, whether a board's state\n\
is a repeat of any previous state in the current game, and whether the\n\
last piece made any captures. If the cursor wrapped or the board is a\n\
repeat, a king spawns. Otherwise, if the last piece made any captures, a\n\
rook, knight, bishop, or queen spawns, depending on the position of the\n\
cursor. Otherwise, a pawn spawns. Each piece can only reach squares it\n\
could reach in chess. Thus, black pawns spawn on the rank 7, white pawns\n\
land on rank 2, and bishops move 2 squares at a time so they stay on their\n\
colors. White pawns capture upward and black pawns capture downward. If a\n\
white pawn lands on rank 8 or a black pawn lands on rank 1, it promotes to\n\
a queen before capturing. The current 'combo' is the number of consecutive\n\
turns on which pieces have been captured. The higher the combo, the faster\n\
pieces fall. The points scored by a given piece are equal to:\n\
[total value of pieces captured] x [number of pieces captured] x [combo]\n\
Piece values: pawn=1 knight=3 bishop=3 rook=5 queen=9 king=0\n\
";
#define RULES_LENGTH 1576