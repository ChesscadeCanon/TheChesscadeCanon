extends Sprite2D

func _draw_square(slot: Vector2i)->void:
	draw_set_transform(Vector2.ZERO, 0, Vector2.ONE * 2)
	var sl := ChesscadeModel.SQUARE_LENGTH
	var white := Color.LIGHT_GRAY
	var black := Color.DARK_GRAY
	var is_white := slot.x % 2 != slot.y % 2
	var color := white if is_white else black
	var rect := Rect2(slot * sl, Vector2.ONE * sl)
	draw_rect(rect, color)	

func _draw_piece(slot: Vector2i):
	draw_set_transform(Vector2.ZERO, 0, Vector2.ONE * 2)
	var sl := ChesscadeModel.SQUARE_LENGTH
	var deck_dir := ChesscadeModel.DECK_PIECES_DIR
	var next_dir := ChesscadeModel.NEXT_PIECES_DIR
	var piece :StringName= ChesscadeModel.get_deck_piece(slot)
	var cursor :Vector2i= ChesscadeModel.get_cursor()
	var is_white := piece.to_upper() == piece
	var dir := next_dir if cursor == slot else deck_dir
	var texture := ChesscadeModel.piece(dir, is_white, piece)
	draw_texture(texture, slot * sl)

func _draw_deck()->void:
	var deck_size :Vector2i= ChesscadeModel.get_deck_size()
	for g in deck_size.y:
		for i in deck_size.x:
			var slot := Vector2i(i, g)
			_draw_square(slot)
			if ChesscadeModel.is_live() and not ChesscadeModel.is_paused():
				_draw_piece(slot)

func _draw()->void:
	_draw_deck()

func _process(delta)->void:
	queue_redraw()
