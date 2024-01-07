import platform
SYSTEM = platform.system().tolower()
ENGINE_PATH = 'bin/ChesscadeLib.dll' if "windows" in SYSTEM else 'bin/ChesscadeLib.so' if "linux" in SYSTEM else 'bin/cygChesscadeLib.dll' if 'cygwin' in SYSTEM else ''
assert(ENGINE_PATH, f"Not compatible with {platform.system()} systems.")
BOARD_PIECES_DIR = "assets/grey_pieces/"
DECK_PIECES_DIR = "assets/black_white_pieces/"
NEXT_PIECES_DIR = "assets/orange_pieces/"
THREATENED_PIECES_DIR = "assets/red_pieces/"
SHADOW_PIECES_DIR = "assets/shadow_pieces/"
PLAYER_PIECES_DIR = "assets/purple_pieces/"