import platform
import ctypes

EVENT_FELL = 1 << 0
EVENT_LEFT = 1 << 2
EVENT_RIGHT = 1 << 3
EVENT_DOWN = 1 << 4
EVENT_DROPPED = 1 << 5
EVENT_WRAPPED = 1 << 6
EVENT_LANDED = 1 << 7
EVENT_CAPTURED = 1 << 8

SYSTEM = platform.system().lower()
ENGINE_PATH = 'bin/ChesscadeLib.dll' if "windows" in SYSTEM else 'bin/libChesscadeLib.so' if "linux" in SYSTEM else 'bin/cygChesscadeLib.dll' if 'cygwin' in SYSTEM else ''
assert len(ENGINE_PATH), f"Not compatible with {platform.system()} systems."

class Game(ctypes.Structure):
    _fields_ = []

def Engine():
    
    ret = ctypes.CDLL(ENGINE_PATH)
    
    ret.attack_pattern.argtypes = [ctypes.POINTER(Game)]
    ret.attack_pattern.restype = ctypes.c_ulonglong
    
    ret.begin_game.argtypes = [ctypes.POINTER(Game)]
    
    ret.delete_game.argtypes = [ctypes.POINTER(Game)]
    
    ret.get_combo.argtypes = [ctypes.POINTER(Game)]
    ret.get_combo.restype = ctypes.c_ulonglong
    
    ret.get_cursor_direction.argtypes = [ctypes.POINTER(Game)]
    ret.get_cursor_direction.restype = ctypes.c_short

    ret.get_cursor_grade.argtypes = [ctypes.POINTER(Game)]
    ret.get_cursor_grade.restype = ctypes.c_ushort
    
    ret.get_cursor_increment.argtypes = [ctypes.POINTER(Game)]
    ret.get_cursor_increment.restype = ctypes.c_ushort
    
    ret.get_deck_piece.argtypes = [ctypes.c_ushort, ctypes.c_ushort]
    ret.get_deck_piece.restype = ctypes.c_char

    ret.get_ease.argtypes = [ctypes.POINTER(Game)]
    ret.get_ease.restype = ctypes.c_longlong
    
    ret.get_events.argtypes = [ctypes.POINTER(Game)]
    ret.get_events.restype = ctypes.c_int
    
    ret.get_files.restype = ctypes.c_ushort
    
    ret.forecast_captures.argtypes = [ctypes.POINTER(Game)]
    ret.forecast_captures.restype = ctypes.c_ulonglong

    ret.get_forecast_piece.argtypes = [ctypes.POINTER(Game)]
    ret.get_forecast_piece.restype = ctypes.c_char
    
    ret.get_forecast_rank.argtypes = [ctypes.POINTER(Game)]
    ret.get_forecast_rank.restype = ctypes.c_ushort
   
    ret.get_move_rate.argtypes = [ctypes.POINTER(Game)]
    ret.get_move_rate.restype = ctypes.c_longlong

    ret.get_next_piece.argtypes = [ctypes.POINTER(Game)]
    ret.get_next_piece.restype = ctypes.c_char
   
    ret.get_player.argtypes = [ctypes.POINTER(Game)]
    ret.get_player.restype = ctypes.c_char
    
    ret.get_player_file.argtypes = [ctypes.POINTER(Game)]
    ret.get_player_file.restype = ctypes.c_ushort

    ret.get_player_rank.argtypes = [ctypes.POINTER(Game)]
    ret.get_player_rank.restype = ctypes.c_ushort
    
    ret.get_ranks.restype = ctypes.c_ushort
    
    ret.get_rules.restype = ctypes.c_char_p
    
    ret.get_rules_length.restype = ctypes.c_ulonglong
    
    ret.get_score.argtypes = [ctypes.POINTER(Game)]
    ret.get_score.restype = ctypes.c_ulonglong
    
    ret.get_scored.argtypes = [ctypes.POINTER(Game)]
    ret.get_scored.restype = ctypes.c_ulonglong
    
    ret.get_spawn_rank.argtypes = [ctypes.POINTER(Game)]
    ret.get_spawn_rank.restype = ctypes.c_ushort

    ret.get_square.argtypes = [ctypes.POINTER(Game), ctypes.c_ushort, ctypes.c_ushort]
    ret.get_square.restype = ctypes.c_char

    ret.get_square_bit.argtypes = [ctypes.c_ushort, ctypes.c_ushort]
    ret.get_square_bit.restype = ctypes.c_ulonglong

    ret.input_analog_move.argtypes = [ctypes.POINTER(Game), ctypes.c_longdouble, ctypes.c_longdouble] 
    
    ret.input_digital_move.argtypes = [ctypes.POINTER(Game), ctypes.c_bool, ctypes.c_bool, ctypes.c_bool]
    
    ret.input_drop.argtypes = [ctypes.POINTER(Game)]
    
    ret.input_toggle_pause.argtypes = [ctypes.POINTER(Game)]
        
    ret.is_game_over.argtypes = [ctypes.POINTER(Game)]
    ret.is_game_over.restype = ctypes.c_bool
    
    ret.is_on_brink.argtypes = [ctypes.POINTER(Game)]
    ret.is_on_brink.restype = ctypes.c_bool
    
    ret.is_paused.argtypes = [ctypes.POINTER(Game)]
    ret.is_paused.restype = ctypes.c_bool
    
    ret.is_repeat.argtypes = [ctypes.POINTER(Game)]
    ret.is_repeat.restype = ctypes.c_bool

    ret.malloc_init_custom_game.restype = ctypes.c_int
    ret.malloc_init_custom_game.restype = ctypes.POINTER(Game)
    
    ret.malloc_init_default_game.restype = ctypes.POINTER(Game)
    
    ret.print_board.argtypes = [ctypes.POINTER(Game)]
    
    ret.pump_game.argtypes = [ctypes.POINTER(Game), ctypes.c_ulonglong]

    return ret