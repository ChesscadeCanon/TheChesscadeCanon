shader_type canvas_item;

uniform int board_state[64];

void vertex() {
	// Called for every vertex the material is visible on.
}

bool axis_parity(float axis) {
	
	return bool(int(floor(axis / (1.0 / 8.0))) % 2);
}

void fragment() {
	COLOR = vec4(1.0) * vec4(float(axis_parity(UV.x) == axis_parity(UV.y)));
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
