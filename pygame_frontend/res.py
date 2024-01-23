import platform
import ctypes

EVENT_FELL = 1 << 0
EVENT_LEFT = 1 << 2
EVENT_RIGHT = 1 << 3
EVENT_DOWN = 1 << 4
EVENT_DROPPED = 1 << 5
EVENT_WRAPPED = 1 << 6

SYSTEM = platform.system().lower()
ENGINE_PATH = 'bin/ChesscadeLib.dll' if "windows" in SYSTEM else 'bin/libChesscadeLib.so' if "linux" in SYSTEM else 'bin/cygChesscadeLib.dll' if 'cygwin' in SYSTEM else ''
assert len(ENGINE_PATH), f"Not compatible with {platform.system()} systems."
BOARD_PIECES_DIR = "assets/grey_pieces/"
DECK_PIECES_DIR = "assets/black_white_pieces/"
NEXT_PIECES_DIR = "assets/orange_pieces/"
THREATENED_PIECES_DIR = "assets/red_pieces/"
SHADOW_PIECES_DIR = "assets/shadow_pieces/"
PLAYER_PIECES_DIR = "assets/purple_pieces/"

engine = ctypes.CDLL(ENGINE_PATH)

engine.get_rules_length.restype = ctypes.c_ulonglong
engine.get_rules.restype = ctypes.c_char_p
engine.get_ranks.restype = ctypes.c_ulonglong
engine.get_files.restype = ctypes.c_ulonglong
engine.get_symbol_count.restype = ctypes.c_ulonglong
engine.get_board_length.restype = ctypes.c_ulonglong

class Game(ctypes.Structure):
    _fields_ = []

engine.malloc_init_default_game.restype = ctypes.POINTER(Game)
engine.input_toggle_pause.argtypes = [ctypes.POINTER(Game)]
engine.input_drop.argtypes = [ctypes.POINTER(Game)]
engine.input_digital_move.argtypes = [ctypes.POINTER(Game), ctypes.c_bool, ctypes.c_bool, ctypes.c_bool]
engine.input_analog_move.argtypes = [ctypes.POINTER(Game), ctypes.c_longdouble, ctypes.c_longdouble]
engine.pump_game.argtypes = [ctypes.POINTER(Game), ctypes.c_ulonglong]
engine.begin_game.argtypes = [ctypes.POINTER(Game)]
engine.is_game_over.argtypes = [ctypes.POINTER(Game)]
engine.is_game_over.restype = ctypes.c_bool
engine.delete_game.argtypes = [ctypes.POINTER(Game)]
engine.get_events.argtypes = [ctypes.POINTER(Game)]
engine.get_events.restype = ctypes.c_ulonglong
engine.get_board.argtypes = [ctypes.POINTER(Game)]
engine.get_board.restype = ctypes.c_char_p
engine.get_player.argtypes = [ctypes.POINTER(Game)]
engine.get_player.restype = ctypes.c_char
engine.get_player_rank.argtypes = [ctypes.POINTER(Game)]
engine.get_player_rank.restype = ctypes.c_ushort
engine.get_player_file.argtypes = [ctypes.POINTER(Game)]
engine.get_player_file.restype = ctypes.c_ushort
engine.get_cursor_grade.argtypes = [ctypes.POINTER(Game)]
engine.get_cursor_grade.restype = ctypes.c_ushort
engine.get_cursor_increment.argtypes = [ctypes.POINTER(Game)]
engine.get_cursor_increment.restype = ctypes.c_ushort
engine.get_next_piece.argtypes = [ctypes.POINTER(Game)]
engine.get_next_piece.restype = ctypes.c_wchar
engine.get_score.argtypes = [ctypes.POINTER(Game)]
engine.get_score.restype = ctypes.c_ulonglong
engine.get_scored.argtypes = [ctypes.POINTER(Game)]
engine.get_scored.restype = ctypes.c_ulonglong
engine.get_combo.argtypes = [ctypes.POINTER(Game)]
engine.get_combo.restype = ctypes.c_ulonglong
engine.forecast_captures.argtypes = [ctypes.POINTER(Game)]
engine.forecast_captures.restype = ctypes.c_ulonglong
engine.get_forecast_piece.argtypes = [ctypes.POINTER(Game)]
engine.get_forecast_piece.restype = ctypes.c_wchar
engine.get_forecast_rank.argtypes = [ctypes.POINTER(Game)]
engine.get_forecast_rank.restype = ctypes.c_ulonglong
engine.attack_pattern.argtypes = [ctypes.POINTER(Game)]
engine.attack_pattern.restype = ctypes.c_ulonglong
engine.get_square_bit.argtypes = [ctypes.c_ulonglong, ctypes.c_ulonglong]
engine.get_square_bit.restype = ctypes.c_ulonglong
engine.get_deck.argtypes = [ctypes.c_ulonglong]
engine.get_deck.restype = ctypes.c_char_p
engine.is_on_brink.argtypes = [ctypes.POINTER(Game)]
engine.is_on_brink.restype = ctypes.c_bool
engine.is_paused.argtypes = [ctypes.POINTER(Game)]
engine.is_paused.restype = ctypes.c_bool
engine.is_repeat.argtypes = [ctypes.POINTER(Game)]
engine.is_repeat.restype = ctypes.c_bool
engine.get_ease.argtypes = [ctypes.POINTER(Game)]
engine.get_ease.restype = ctypes.c_ulonglong