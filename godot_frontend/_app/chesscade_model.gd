extends ChesscadeAdapter
	
func debug_board()->void:
	print(get_board_state())
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pump_game(delta)

	
