shader_type canvas_item;

const int EMPTY = 95;
const uint SQUARE_LENGTH = 16u;
const uint FIGURE_HEIGHT = 4u;

uniform uvec2 board_size;

void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {

	uvec2 square = uvec2(UV * vec2(board_size));
	bool white_square = square.x % 2u == square.y % 2u;
	vec4 board_color = vec4(float(white_square), float(white_square), float(white_square), 1.0);
	COLOR = board_color;
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
