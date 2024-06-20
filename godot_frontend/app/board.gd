extends Panel

func _draw_piece_on_square(dir:StringName, white: bool, piece: StringName, square: Vector2i):
	var texture := ChesscadeModel.piece(dir, white, piece)
	draw_texture(texture, square * ChesscadeModel.SQUARE_LENGTH)

func _draw_landed_pieces()->void:
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var board_state_string :String= ChesscadeModel.get_board_state()
	var board_state_array :PackedByteArray= board_state_string.to_ascii_buffer()
	var index := 0
	for p in board_state_string:
		var square = Vector2i(index % (board_size.x + 1), index / (board_size.x + 1))
		if p.to_lower() in ChesscadeModel.PIECE_NAMES: 
			var white := p.to_upper() == p
			var dir :=ChesscadeModel.BOARD_PIECES_DIR
			_draw_piece_on_square(dir, white, p, square)
		index += 1

func _draw_player()->void:
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var player_string :StringName= ChesscadeModel.get_player()
	var white := player_string.to_upper() == player_string
	var dir :=ChesscadeModel.BOARD_PIECES_DIR
	_draw_piece_on_square(dir, white, player_string, player_square)

func _process(delta)->void:
	queue_redraw()
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _draw()->void:
	_draw_player()
	_draw_landed_pieces()
