shader_type canvas_item;

const int EMPTY = 95;
const uint SQUARE_LENGTH = 16u;
const uint FIGURE_HEIGHT = 4u;

uniform uvec2 board_size;
uniform int board_state[72];
uniform uvec2 player_square;
uniform int player;
uniform sampler2DArray piece_map;
uniform sampler2D bishop;
uniform sampler2D king;
uniform sampler2D knight;
uniform sampler2D pawn;
uniform sampler2D queen;
uniform sampler2D rook;
const int BISHOP = 66;
const int KING = 75;
const int KNIGHT = 78;
const int PAWN = 80;
const int QUEEN = 81;
const int ROOK = 82;

void vertex() {
	// Called for every vertex the material is visible on.
}

vec4 piece_color(const sampler2D piece_map[128], const vec2 uv, const int piece) {
	
	vec4 ret = texture(piece_map[piece], uv * vec2(board_size));
	return ret;
}

void fragment() {

	uvec2 square = uvec2(UV * vec2(board_size));
	vec2 origin = vec2(square) / vec2(board_size);
	vec2 texture_uv = (UV - origin) * vec2(board_size);
	ivec2 texture_iv = ivec2(texture_uv * float(SQUARE_LENGTH));
	uint square_index = square.y * (board_size.x + 1u) + square.x;
	int occupant = board_state[square_index];
	bool has_occupant = bool(occupant);
	bool has_player = square == player_square;
	bool has_piece = has_occupant || has_player;
	bool white_square = square.x % 2u == square.y % 2u;
	vec4 board_color = vec4(float(white_square), float(white_square), float(white_square), 1.0);
	vec4 piece_color = float(has_player) * texelFetch(pawn, texture_iv, 0);
	COLOR = bool(piece_color.a) ? piece_color : board_color;
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
