extends Panel

# Called when the node enters the scene tree for the first time.
func _ready():
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	material.set_shader_parameter("board_size", board_size)
	var sprite_map := ChesscadeModel.PIECE_MAP
	material.set_shader_parameter("piece_map", sprite_map)
	material.set_shader_parameter("pawn", ChesscadeModel.PAWN)
	
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
