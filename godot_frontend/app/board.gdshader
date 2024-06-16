shader_type canvas_item;

const int EMPTY = 95;

uniform uvec2 board_size;
uniform uvec4 pixel_map[128];
uniform uvec4 mask_map[128];
uniform int board_state[72];
uniform uvec2 player_square;

varying flat uvec2 square;
varying flat uint index;

void vertex() {
	// Called for every vertex the material is visible on.
}

bool white_square() {
	
	return square.x % 2u == square.y % 2u;
}

bool occupied() {
	
	return board_state[index] != EMPTY;
}

bool player() {
	
	return square == player_square;
}

bool be_white() {
	
	if(player()) {
		
		return !white_square();
	} else if(occupied()) {
		
		return !white_square();
	} else {
	
		return white_square();
	}
}

void fragment() {

	square = uvec2(uint(UV.x * float(board_size.x)), uint(UV.y * float(board_size.y)));
	index = square.y * (board_size.x + 1u) + square.x;
	COLOR = vec4(float(be_white()));
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
