extends ChesscadeAdapter

const KING := preload("res://bk.png")
const QUEEN := preload("res://bq.png")
const BISHOP := preload("res://bb.png")
const KNIGHT := preload("res://bn.png")
const ROOK := preload("res://br.png")
const PAWN := preload("res://bp.png")
var PIECE_MAP := {}

func debug_board()->void:
	print(get_board_state())

func _init():
	PIECE_MAP['k'] = KING
	PIECE_MAP['q'] = QUEEN
	PIECE_MAP['b'] = BISHOP
	PIECE_MAP['q'] = KNIGHT
	PIECE_MAP['r'] = ROOK		
	PIECE_MAP['p'] = PAWN

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pump_game(delta)
	debug_board()

