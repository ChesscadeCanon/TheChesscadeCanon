extends Sprite2D

func _draw_piece_on_square(dir:StringName, white: bool, piece: StringName, square: Vector2i):
	var texture := ChesscadeModel.piece(dir, white, piece)
	draw_texture(texture, square * ChesscadeModel.SQUARE_LENGTH)

func _draw_landed_pieces()->void:
	var threats :int= ChesscadeModel.forecast_captures()
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var board_state_string :String= ChesscadeModel.get_board_state()
	var board_state_array :PackedByteArray= board_state_string.to_ascii_buffer()
	var index := 0
	for p in board_state_string:
		var rank := index / (board_size.x + 1)
		var file := index % (board_size.x + 1)
		var square = Vector2i(file, rank)
		var bit := ChesscadeModel.get_square_bit(square)
		if p.to_lower() in ChesscadeModel.PIECE_NAMES:
			var white := p.to_upper() == p
			var dir: StringName
			if threats & bit:
				dir = ChesscadeModel.THREATENED_PIECES_DIR
			else:
				dir = ChesscadeModel.BOARD_PIECES_DIR
			_draw_piece_on_square(dir, white, p, square)
		index += 1

func _draw_player()->void:
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var player_string :StringName= ChesscadeModel.get_player()
	var white := player_string.to_upper() == player_string
	var dir := ChesscadeModel.PLAYER_PIECES_DIR
	_draw_piece_on_square(dir, white, player_string, player_square)

func _draw_forecast()->void:
	var forecast_rank :int= ChesscadeModel.get_forecast_rank()
	var player_string :StringName= ChesscadeModel.get_player()
	var player_square :Vector2i= ChesscadeModel.get_player_square()
	var square := Vector2i(player_square.x, forecast_rank)
	var white := player_string.to_upper() == player_string
	var dir := ChesscadeModel.SHADOW_PIECES_DIR
	_draw_piece_on_square(dir, white, player_string, square)

func _draw_cursor()->void:
	var sl := ChesscadeModel.SQUARE_LENGTH
	var cursor_direction :int= ChesscadeModel.get_cursor_direction()
	var spawn_rank :int= ChesscadeModel.get_spawn_rank()
	var cursor :Vector2i= ChesscadeModel.get_cursor()
	var square := Vector2i(cursor.x, spawn_rank) * sl
	var src := Rect2(Vector2.ZERO, Vector2.ONE * sl)
	var dest := Rect2(square, Vector2.ONE * cursor_direction * sl)
	var texture := ChesscadeModel.ARROW_TEXTURE
	draw_texture_rect_region(texture, dest, src, Color.WHITE)
	
func _draw_board()->void:
	var white := Color.WHITE
	var black := Color.BLACK
	var live := ChesscadeModel.is_live()
	
	if not live:
		white = Color.LIGHT_GRAY
		black = Color.DARK_GRAY
	elif ChesscadeModel.is_on_brink():
		white = Color.ORANGE
		black = Color.ORANGE_RED
	elif ChesscadeModel.is_repeat():
		white = Color.AQUA
		black = Color.DARK_BLUE
		
	var pattern_white := Color.DARK_SALMON
	var pattern_black := Color.DARK_RED
	
	var attack_pattern :int= ChesscadeModel.attack_pattern()
	
	var board_size :Vector2i= ChesscadeModel.get_board_size()
	var sl := ChesscadeModel.SQUARE_LENGTH
	var rect := Rect2(Vector2.ZERO, Vector2(sl, sl))
	for r in board_size.y:
		for f in board_size.x:
			var bit := ChesscadeModel.get_square_bit(Vector2i(f, r))
			var attacked :bool= attack_pattern & bit
			var is_white := r % 2 == f % 2
			var color := white if is_white else black
			var pattern_color := pattern_white if is_white else pattern_black
			var final_color := pattern_color if attacked and live else color
			rect.position = Vector2(f, r) * sl
			draw_rect(rect, final_color)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _draw()->void:
	_draw_board()
	if ChesscadeModel.is_live():
		_draw_forecast()
		_draw_cursor()
		_draw_landed_pieces()
		_draw_player()

func _process(delta)->void:
	queue_redraw()
