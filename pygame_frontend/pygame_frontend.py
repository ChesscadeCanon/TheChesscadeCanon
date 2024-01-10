import ctypes
import atexit
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
DARK_GREY = (63, 63, 63)
LIGHT_GREY = (171, 171, 171)
SIZE = (400, 480)
RANKS = engine.get_ranks()
FILES = engine.get_files()
TRIE_CHILDREN = engine.get_trie_children()
STATE_LENGTH = engine.get_state_length()
SQUARES_OFF_LEFT = 2
SQUARES_OFF_TOP = 4
SQUARE = min(SIZE[0] // (FILES + SQUARES_OFF_LEFT), SIZE[1] // (RANKS + SQUARES_OFF_TOP))
FONT_1 = pygame.font.Font('freesansbold.ttf', 16)
                                                 
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
        ("player", ctypes.c_char),
        ("player_rank", ctypes.c_ushort),
        ("player_file", ctypes.c_ushort),
        ("cursor", ctypes.c_short),
        ("cursor_rank", ctypes.c_ushort),
        ("cursor_file", ctypes.c_ushort),
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

screen = pygame.display.set_mode(SIZE)
pygame.display.set_caption("Chesscade")

PIECES = ['p', 'q', 'k', 'n', 'b', 'r']
piece_path = lambda p, d: (d + ('w' + p if p.isupper() else 'b' + p)).lower() + ".png"
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
shadow_pieces = load_pieces(SHADOW_PIECES_DIR)
player_pieces = load_pieces(PLAYER_PIECES_DIR)
draw_piece = lambda P, p, x, y: p in P and screen.blit(P[p], (x, y))
draw_piece_on_square = lambda P, p, r, f: draw_piece(P, p, (f + SQUARES_OFF_LEFT) * SQUARE, (r + SQUARES_OFF_TOP) * SQUARE)
draw_piece_on_deck = lambda P, p, r, f: draw_piece(P, p, (f + SQUARES_OFF_LEFT) * SQUARE, r * SQUARE)

def draw_board(game):
    color = WHITE
    pattern = engine.attack_pattern(game)
    for f in range(FILES):
        for r in range(RANKS):
            i = r * FILES + f
            b = 1 << i
            x = (SQUARES_OFF_LEFT + f) * SQUARE
            y = (SQUARES_OFF_TOP + r) * SQUARE
            grey = LIGHT_GREY if color == WHITE else DARK_GREY
            pygame.draw.rect(screen, grey if b & pattern else color, [x, y, SQUARE, SQUARE], 0)
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
    player = game.contents.player.decode('ascii')
    rank = game.contents.player_rank
    file = game.contents.player_file
    draw_piece_on_square(player_pieces, player, rank, file)

def draw_next(game):
    cursor_rank = game.contents.cursor_rank
    cursor_file = game.contents.cursor_file
    next_piece = engine.get_next_piece(game)
    for g in range(SQUARES_OFF_TOP):
        deck = [chr(c) for c in engine.get_deck(g)]
        draw_piece_on_deck(deck_pieces, deck[0], g, 0)
        [draw_piece_on_deck(deck_pieces, deck[i], g, i) for i in range(FILES)]
    draw_piece_on_deck(next_pieces, next_piece, cursor_rank, cursor_file)

def draw_shadow(game):
    piece = engine.get_forecast_piece(game)
    rank = engine.get_forecast_rank(game)
    file = game.contents.player_file
    draw_piece_on_square(shadow_pieces, piece, rank, file)

def draw_text(game):
    readouts = ("Score", f"{game.contents.score}"), ("Scored", f"{game.contents.scored}"), ("Combo", f"{game.contents.combo}")
    y = 0
    for readout in readouts:
        for part in readout:    
            text = FONT_1.render(part, True, WHITE, BLACK)
            rect = text.get_rect()
            rect.topright = (SQUARE * SQUARES_OFF_LEFT, y)
            screen.blit(text, rect)
            y += text.get_rect().height
        y += text.get_rect().height

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
    ret = 0
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
        carryOn = carryOn and not engine.is_game_over(game)
        screen.fill(GREY)
        draw_board(game)
        draw_next(game)
        draw_pieces(game)
        draw_shadow(game)
        draw_player(game)
        draw_text(game)
        pygame.display.flip()
        clock.tick(60)
    ret = game.contents.score
    engine.delete_game(game)
    return ret if engine.is_game_over(game) else -1
high_score = 0
while True:
    score = play()
    if score < 0:
        break
    if score > high_score:
        high_score = score
    print(score)
print(high_score)
pygame.quit()

    