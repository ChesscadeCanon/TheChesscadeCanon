@tool
extends PanelContainer

# Called when the node enters the scene tree for the first time.
func _ready():
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var pixel_map :Array[Vector4i]= []
	var mask_map :Array[Vector4i]= []
	for code in 128:
		var piece := char(code)
		var pixel_vector := Vector4i(0, 0, 0, 0)
		var mask_vector := Vector4i(0, 0, 0, 0)
		for chunk in 4:
			var pixels :int= ChesscadeModel.get_figure(piece, 0, chunk)
			pixel_vector[chunk] = pixels
			var mask :int= ChesscadeModel.get_figure(piece, 1, chunk)
			mask_vector[chunk] = mask
		pixel_map.append(pixel_vector)
		mask_map.append(mask_vector)
	material.set_shader_parameter("board_size", board_size)
	material.set_shader_parameter("pixel_map", pixel_map)
	material.set_shader_parameter("mask_map", mask_map)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta)->void:
	var board_state_string :StringName= ChesscadeModel.get_board_state()
	var board_state_array :PackedByteArray= board_state_string.to_ascii_buffer()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	material.set_shader_parameter("board_state", board_state_array)
	material.set_shader_parameter("player_square", player_square)
