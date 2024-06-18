extends ChesscadeAdapter

const KING := preload("res://bk.png")
const QUEEN := preload("res://bq.png")
const BISHOP := preload("res://bb.png")
const KNIGHT := preload("res://bn.png")
const ROOK := preload("res://br.png")
const PAWN := preload("res://bp.png")
var PIECE_MAP :Array[Texture2D] = []

func debug_board()->void:
	print(get_board_state())

func ord(s: String)->int:
	if len(s):
		return s.to_ascii_buffer()[0]
	return 0

func _init():
	PIECE_MAP.resize(128)
	PIECE_MAP[ord('k')] = KING
	PIECE_MAP[ord('q')] = QUEEN
	PIECE_MAP[ord('b')] = BISHOP
	PIECE_MAP[ord('q')] = KNIGHT
	PIECE_MAP[ord('r')] = ROOK		
	PIECE_MAP[ord('p')] = PAWN

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pump_game(delta)
	debug_board()

	
