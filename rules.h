#pragma once
#define RULES "\
Chesscade is a falling block puzzle game with chess pieces. Pieces fall from the\n\
top of the board. When a piece reaches the bottom, it captures all pieces it\n\
could capture in chess from its position. When a king is captured or the next\n\
piece can't spawn, the game ends. As a piece falls, a cursor moves across the\n\
top of the board. When it reaches one side, it wraps around to the other. When\n\
a capture is made, the cursor changes directions. When a piece lands, the next\n\
piece is determined by where the cursor is, whether the cursor wrapped around,\n\
and whether the last piece made any captures. If the cursor wrapped, a king\n\
spawns. Otherwise, if the last piece made any captures, a rook, knight, bishop,\n\
or queen spawns, depending on the position of the cursor. Otherwise, a pawn\n\
spawns. Each piece can only reach squares it could reach in chess. Thus, black\n\
pawns spawn on the rank 7, white pawns land on rank 2, and bishops move 2\n\
squares at a time so they stay on their colors. White pawns capture upward and\n\
black pawns capture downward. If a white pawn lands on rank 8 or a black pawn\n\
lands on rank 1, it promotes to a queen before capturing. The current \"combo\"\n\
is the number of consecutive turns on which pieces have been captured. The\n\
higher the combo, the faster pieces fall. No points can be scored if the board\n\
state is identical to any previous board state in the current game. The points\n\
scored by a given piece are equal to:\n\
[total point value of pieces captured] x [number of pieces captured] x [combo]\n\
Point values are as follows: pawn=1 knight=3 bishop=3 rook=5 queen=9 king=0\n\
"