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

class Histotrie(ctypes.Structure):
    pass
Histotrie._fields_ = [('children', ctypes.POINTER(Histotrie) * engine.get_symbol_count())]

class Game(ctypes.Structure):
    _fields_ = [
        ("score", ctypes.c_ulonglong),
        ("combo", ctypes.c_ulonglong),
        ("scored", ctypes.c_ulonglong),
        ("time", ctypes.c_ulonglong),
        ("fell", ctypes.c_ulonglong),
        ("paused", ctypes.c_bool),
        ("player", ctypes.c_char),
        ("player_rank", ctypes.c_ushort),
        ("player_file", ctypes.c_ushort),
        ("cursor", ctypes.c_short),
        ("cursor_grade", ctypes.c_ushort),
        ("cursor_increment", ctypes.c_ushort),
        ("dropped", ctypes.c_bool),
        ("moved_left", ctypes.c_long),
        ("moved_right", ctypes.c_long),
        ("moved_down", ctypes.c_long),
        ("board", ctypes.c_char * engine.get_board_length()),
        ("captures", ctypes.c_char * engine.get_files()),
        ("histotrie", ctypes.POINTER(Histotrie)),
        ("repeat", ctypes.c_bool),
        ("settings", ctypes.c_ulonglong),
        ("events", ctypes.c_ulonglong),
        ("total_pieces", ctypes.c_ulonglong),
        ("total_value", ctypes.c_ulonglong)
    ]

engine.malloc_init_default_game.restype = ctypes.POINTER(Game)
engine.increment_game.argtypes = [ctypes.POINTER(Game), ctypes.c_ulonglong]
engine.begin_game.argtypes = [ctypes.POINTER(Game)]
engine.is_game_over.argtypes = [ctypes.POINTER(Game)]
engine.is_game_over.restype = ctypes.c_bool
engine.delete_game.argtypes = [ctypes.POINTER(Game)]
engine.get_next_piece.argtypes = [ctypes.POINTER(Game)]
engine.get_next_piece.restype = ctypes.c_wchar
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
engine.get_ease.argtypes = [ctypes.POINTER(Game)]
engine.get_ease.restype = ctypes.c_ulonglong