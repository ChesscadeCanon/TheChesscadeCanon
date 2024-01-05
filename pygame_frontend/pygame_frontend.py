import ctypes
import pygame
from res import *

pygame.init()
engine = ctypes.CDLL(ENGINE_PATH)

engine.get_ranks.restype = ctypes.c_ulonglong
engine.get_files.restype = ctypes.c_ulonglong
engine.get_trie_children.restype = ctypes.c_ulonglong
engine.get_state_length.restype = ctypes.c_ulonglong

BLACK = ( 0, 0, 0)
WHITE = ( 255, 255, 255)
GREY = ( 127, 127, 127)
SIZE = (576, 576)
RANKS = engine.get_ranks()
FILES = engine.get_files()
TRIE_CHILDREN = engine.get_trie_children()
STATE_LENGTH = engine.get_state_length()
SQUARE = min(SIZE[0] // (FILES + 1), SIZE[1] // (RANKS + 1))
                                                 
class Histotrie(ctypes.Structure):
    pass
Histotrie._fields_ = [('children', ctypes.POINTER(Histotrie) * TRIE_CHILDREN)]

class Game(ctypes.Structure):
    _fields_ = [
        ("score", ctypes.c_ulonglong),
        ("combo", ctypes.c_ulonglong),
        ("scored", ctypes.c_ulonglong),
        ("time", ctypes.c_ulonglong),
        ("fell", ctypes.c_ulonglong),
        ("paused", ctypes.c_bool),
        ("dropped", ctypes.c_bool),
        ("moved_left", ctypes.c_long),
        ("moved_right", ctypes.c_long),
        ("moved_down", ctypes.c_long),
        ("state", ctypes.c_char * STATE_LENGTH),
        ("histotrie", ctypes.POINTER(Histotrie)),
        ("settings", ctypes.c_ulonglong),
        ("repeat", ctypes.c_bool),
    ]
    
engine.malloc_init_default_game.restype = ctypes.POINTER(Game)
engine.increment_game.argtypes = [ctypes.POINTER(Game), ctypes.c_ulonglong]
engine.begin_game.argtypes = [ctypes.POINTER(Game)]
engine.game_over.argtypes = [ctypes.POINTER(Game)]
engine.game_over.restype = ctypes.c_bool
engine.delete_game.argtypes = [ctypes.POINTER(Game)]
engine.get_player.argtypes = [ctypes.POINTER(Game)]
engine.get_player.restype = ctypes.c_wchar
engine.get_player_rank.argtypes = [ctypes.POINTER(Game)]
engine.get_player_rank.restype = ctypes.c_ulonglong
engine.get_player_file.argtypes = [ctypes.POINTER(Game)]
engine.get_player_file.restype = ctypes.c_ulonglong
engine.get_cursor.argtypes = [ctypes.POINTER(Game)]
engine.get_cursor.restype = ctypes.c_wchar
engine.get_cursor_rank.argtypes = [ctypes.POINTER(Game)]
engine.get_cursor_rank.restype = ctypes.c_ulonglong
engine.get_cursor_file.argtypes = [ctypes.POINTER(Game)]
engine.get_cursor_file.restype = ctypes.c_ulonglong
engine.get_next_piece.argtypes = [ctypes.POINTER(Game)]
engine.get_next_piece.restype = ctypes.c_wchar
engine.forecast_captures.argtypes = [ctypes.POINTER(Game)]
engine.forecast_captures.restype = ctypes.c_ulonglong
engine.get_square_bit.argtypes = [ctypes.c_ulonglong, ctypes.c_ulonglong]
engine.get_square_bit.restype = ctypes.c_ulonglong
engine.get_deck.restype = ctypes.c_wchar_p

screen = pygame.display.set_mode(SIZE)
pygame.display.set_caption("Chesscade")

PIECES = ['p', 'q', 'k', 'n', 'b', 'r']
piece_path = lambda p, d: d + ('w' + p if p.isupper() else 'b' + p) + ".png"
load_piece = lambda n, d: pygame.image.load(piece_path(n, d))
scale_piece = lambda p: pygame.transform.scale(p, (SQUARE, SQUARE))
setup_piece = lambda n, d: scale_piece(load_piece(n, d))

def tint_piece(p, n):
    pixels = pygame.PixelArray(p)
    pixels.replace(BLACK if n.isupper() else WHITE, GREY)
    del pixels
    return p

def load_pieces(directory):
    ret = {}
    [ret.update({p: setup_piece(p, directory)}) for p in PIECES + list([w.upper() for w in PIECES])]
    return ret

board_pieces = load_pieces(BOARD_PIECES_DIR)
deck_pieces = load_pieces(DECK_PIECES_DIR)
next_pieces = load_pieces(NEXT_PIECES_DIR)
threatened_pieces = load_pieces(THREATENED_PIECES_DIR)
draw_piece = lambda P, p, x, y: p in P and screen.blit(P[p], (x, y))
draw_piece_on_square = lambda P, p, r, f: draw_piece(P, p, (f + 1) * SQUARE, (r + 1) * SQUARE)

def draw_board():
    color = WHITE
    for x in range(SQUARE, SIZE[0], SQUARE):
        for y in range(SQUARE, SIZE[1], SQUARE):
            pygame.draw.rect(screen, color, [x, y, SQUARE, SQUARE], 0)
            color = BLACK if color == WHITE else WHITE
        color = BLACK if color == WHITE else WHITE

def draw_pieces(game):
    index = 0
    threats = engine.forecast_captures(game)
    for r in range(RANKS):
        for f in range(FILES):
            while chr(game.contents.state[index]) == '\n':
                index += 1
            square = chr(game.contents.state[index])
            if square in board_pieces:
                if threats & engine.get_square_bit(r, f):
                    draw_piece_on_square(threatened_pieces, square, r, f)
                else:
                    draw_piece_on_square(board_pieces, square, r, f)
            index += 1

def draw_player(game):
    player = engine.get_player(game)
    rank = engine.get_player_rank(game)
    file = engine.get_player_file(game)
    draw_piece_on_square(board_pieces, player, rank, file)

#const char DECK[2][9] = {
#	"rNbQqBnR",
#	"PpPpPpPp"
#};

def draw_next(game):
    cursor_rank = engine.get_cursor_rank(game)
    cursor_file = engine.get_cursor_file(game)
    next_piece = engine.get_next_piece(game)
    nexts = ()
    if next_piece in 'Pp':
        nexts = 'PpPpPpPp'
    elif next_piece in 'Kk':
        if cursor_rank:
            nexts = 'KkKkKkKk'
        else:
            nexts = 'kKkKkKkK'
    else:
        nexts = 'rNbQqBnR'
    [draw_piece(deck_pieces, nexts[i], (i+1) * SQUARE, 0) for i in range(FILES)]
    draw_piece_on_square(next_pieces, next_piece, -1, cursor_file)

def take_input(game, passed):
    keys=pygame.key.get_pressed()
    
    if keys[pygame.K_LEFT]:
        game.contents.moved_left += passed
    else:
        game.contents.moved_left = -1    
        
    if keys[pygame.K_RIGHT]:
        game.contents.moved_right += passed
    else:
        game.contents.moved_right = -1     
        
    if keys[pygame.K_DOWN]:
        game.contents.moved_down += passed
    else:
        game.contents.moved_down = -1    

def play():
    game = engine.malloc_init_default_game()
    engine.begin_game(game)
    carryOn = True
    clock = pygame.time.Clock()
    while carryOn:
        for event in pygame.event.get(): 
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    game.contents.dropped = True
            if event.type == pygame.QUIT: 
                carryOn = False 
        passed = clock.tick(30)
        take_input(game, passed)
        engine.increment_game(game, passed)
        carryOn = carryOn and not engine.game_over(game)
        screen.fill(GREY)
        draw_board()
        draw_next(game)
        draw_pieces(game)
        draw_player(game)
        pygame.display.flip()
        clock.tick(60)
    engine.delete_game(game)
    pygame.quit()
 
play()