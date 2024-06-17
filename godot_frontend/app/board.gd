extends Panel

# Called when the node enters the scene tree for the first time.
func _ready():
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var sprite_map :Array[Vector4i]= []
	var mask_map :Array[Vector4i]= []
	for code in 128:
		var piece := char(code)
		var sprite_vector := Vector4i(0, 0, 0, 0)
		var mask_vector := Vector4i(0, 0, 0, 0)
		for word in 4:
			var sprite :int= ChesscadeModel.get_figure(piece, 0, word)
			sprite_vector[word] = sprite
			var mask :int= ChesscadeModel.get_figure(piece, 1, word)
			mask_vector[word] = mask
		sprite_map.append(sprite_vector)
		mask_map.append(mask_vector)
	material.set_shader_parameter("board_size", board_size)
	material.set_shader_parameter("sprite_map", sprite_map)
	material.set_shader_parameter("mask_map", mask_map)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta)->void:
	var board_state_string :StringName= ChesscadeModel.get_board_state()
	var board_state_array :PackedByteArray= board_state_string.to_ascii_buffer()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var player_string :StringName= ChesscadeModel.get_player()
	var player_bytes :PackedByteArray= player_string.to_ascii_buffer()
	var player :int= player_bytes[0]
	material.set_shader_parameter("board_state", board_state_array)
	material.set_shader_parameter("player_square", player_square)
	material.set_shader_parameter("player", player)
