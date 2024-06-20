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
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var player_string :StringName= ChesscadeModel.get_player()
	var white := player_string.to_upper() == player_string
	var dir :=ChesscadeModel.BOARD_PIECES_DIR
	_draw_piece_on_square(dir, white, player_string, player_square)

func _draw_forecast()->void:
	var forecast_rank :int= ChesscadeModel.get_forecast_rank()
	var player_string :StringName= ChesscadeModel.get_player()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var square := Vector2i(player_square.x, forecast_rank)
	var white := player_string.to_upper() == player_string
	var dir := ChesscadeModel.SHADOW_PIECES_DIR
	_draw_piece_on_square(dir, white, player_string, square)

func _draw_board()->void:
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var sl := ChesscadeModel.SQUARE_LENGTH
	var rect := Rect2(Vector2.ZERO, Vector2(sl, sl))
	for r in board_size.y:
		for f in board_size.x:
			rect.position = Vector2(r, f) * sl
			var white := r % 2 == f % 2
			var color := Color.WHITE if white else Color.BLACK
			draw_rect(rect, color)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _draw()->void:
	_draw_board()
	_draw_player()
	_draw_landed_pieces()
	_draw_forecast()

func _process(delta)->void:
	queue_redraw()
