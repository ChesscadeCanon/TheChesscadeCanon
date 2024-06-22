#pragma once
static const char* RULES = "\
Chesscade is a falling block puzzle game with chess pieces. Pieces fall\n\
from the top of the board. When a piece reaches the bottom, it captures\n\
all pieces it could capture in chess from its position. When a king is\n\
captured or the next piece can't spawn, the game ends. As a piece falls,\n\
a cursor moves across the top of the board. When it reaches one side, it\n\
wraps around to the other. When a capture is made, the cursor changes\n\
directions. When a piece lands, the next piece is determined by where\n\
the cursor is, whether the cursor wrapped around, whether the board's\n\
state is a repeat of any previous state in the current game, and whether\n\
the last piece made any captures. If the cursor wrapped or the board is\n\
a repeat, a king spawns. Otherwise, if the last piece made any captures,\n\
a rook, knight, bishop, or queen spawns, depending on the position of\n\
the cursor. Otherwise, a pawn spawns. Each piece can only reach squares\n\
it could reach in chess. Thus, black pawns spawn on the rank 7, white\n\
pawns land on rank 2, and bishops move 2 squares at a time so they stay\n\
on their colors. White pawns capture upward and black pawns capture\n\
downward. If a white pawn lands on rank 8 or a black pawn lands on rank\n\
1, it promotes to a queen before capturing. The points scored when a\n\
piece lands are equal to:\n\
\n\
([total value of pieces captured] + [pieces same color as landed piece])\n\
x ([number of pieces captured] + [rank piece landed, inverse if white])\n\
\n\
Piece values: pawn=1 knight=3 bishop=3 rook=5 queen=9 king=0\n\
";
#define RULES_LENGTH 1536