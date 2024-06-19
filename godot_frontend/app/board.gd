extends Panel

# Called when the node enters the scene tree for the first time.
func _ready():

	var sprite_map := ChesscadeModel.PIECE_MAP

func _process(delta)->void:
	queue_redraw()
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _draw()->void:
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var board_state_string :StringName= ChesscadeModel.get_board_state()
	var board_state_array :PackedByteArray= board_state_string.to_ascii_buffer()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var player_string :StringName= ChesscadeModel.get_player()
	var player_texture :Texture2D= ChesscadeModel.PIECE_MAP[player_string.to_lower()]
	draw_texture(player_texture, player_square * 16.0)
	
