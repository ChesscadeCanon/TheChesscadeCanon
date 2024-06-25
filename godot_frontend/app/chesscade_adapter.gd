extends ChesscadeAdapter

const PIECE_NAMES := "bknpqr"
const SQUARE_LENGTH := 16
const BOARD_PIECES_DIR :StringName= "grey_pieces"
const DECK_PIECES_DIR :StringName= "black_white_pieces"
const NEXT_PIECES_DIR :StringName= "orange_pieces"
const THREATENED_PIECES_DIR :StringName= "red_pieces"
const SHADOW_PIECES_DIR :StringName= "shadow_pieces"
const PLAYER_PIECES_DIR :StringName= "purple_pieces"
const ARROW_TEXTURE := preload("res://assets/cursor.png")

var _piece_textures := {}

func is_live()->bool:
	return not is_paused() and get_time() != 0.0

func pause()->void:
	if not is_paused():
		input_toggle_pause()
		
func unpause()->void:
	if is_paused():
		input_toggle_pause()

func piece(dir:StringName, white: bool, piece: StringName)->Texture2D:
	var color := 'w' if white else 'b'
	var path :StringName= 'res://assets/' + dir + '/' + color + piece.to_lower() + '.png'
	if not _piece_textures.find_key(path):
		_piece_textures[path] = load(String(path))
	return _piece_textures[path]

func debug_board()->void:
	print(get_board_state())

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if not is_game_over() and not is_paused():
		pump_game(delta)

