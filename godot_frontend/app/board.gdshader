shader_type canvas_item;

const int EMPTY = 95;
const uint SQUARE_LENGTH = 16u;
const uint FIGURE_HEIGHT = 4u;

uniform uvec2 board_size;
uniform uvec4 sprite_map[128];
uniform uvec4 mask_map[128];
uniform int board_state[72];
uniform uvec2 player_square;
uniform int player;

void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {

	uvec2 square = uvec2(UV * vec2(board_size));
	vec2 uv = UV - (vec2(square) / vec2(board_size));
	uvec2 pixel = uvec2(uv * vec2(board_size * SQUARE_LENGTH));
	uint square_index = square.y * (board_size.x + 1u) + square.x;
	uint word = pixel.y / 4u;
	uint chunk = pixel.y % 4u;
	uint offset = (SQUARE_LENGTH * chunk) + pixel.x;
	uint flag = (1u << 63u) >> offset + 0xffffffffu;
	int occupant = board_state[square_index];
	bool has_occupant = bool(occupant);
	bool has_player = square == player_square;
	bool has_piece = has_occupant || has_player;
	uvec4 sprite = has_player ? sprite_map[player] : sprite_map[occupant];
	uvec4 mask = has_player ? mask_map[player] : mask_map[occupant];
	bool in_mask = bool(mask[word] & flag);
	bool sprite_white = bool(sprite[word] & flag);
	bool white_square = square.x % 2u == square.y % 2u;
	bool white = has_piece && in_mask ? sprite_white : white_square; //bool(mask[word] & flag) ? bool(sprite[word] & flag) : white_square;
	vec4 color = vec4(float(white), float(white), float(white), 1.0);
	
	if(chunk % 2u == 0u && sprite_white) {
		COLOR = vec4(0.0,1.0,0.0,1.0);
	} else if(chunk % 2u == 1u && sprite_white) {
		COLOR = vec4(1.0,0.0,0.0,1.0);
	} else {
		COLOR = color;
	}
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
