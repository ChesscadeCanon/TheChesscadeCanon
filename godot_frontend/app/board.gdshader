shader_type canvas_item;

const int EMPTY = 95;

uniform uvec2 board_size;
uniform uvec4 pixel_map[128];
uniform uvec4 mask_map[128];
uniform int board_state[72];
uniform uvec2 player_square;

void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {

	uvec2 square = uvec2(uint(UV.x * float(board_size.x)), uint(UV.y * float(board_size.y)));
	uint index = square.y * (board_size.x + 1u) + square.x;
	COLOR = vec4(float(square.x % 2u == square.y % 2u));
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
