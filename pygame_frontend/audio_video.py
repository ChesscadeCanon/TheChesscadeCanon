import pygame
from os import environ
from lexicon import *

pygame.init()

if "REPL_OWNER" not in environ:
    import pygame.midi
    pygame.midi.init()
    from instrument import MIDINote

    fall_notes = [
        list(range(32, 48, 2)),
        list(range(48, 64, 2)),
        list(range(64, 80, 2)),
        list(range(80, 96, 2))
    ]
    
    port = pygame.midi.get_default_output_id()
    midi_out = pygame.midi.Output(port, 0)
    midi_out.set_instrument(0)
    fall_notes = [[MIDINote(midi_out, n, 64) for n in s] for s in fall_notes]
    
FONT_0 = pygame.font.Font('freesansbold.ttf', 11)
FONT_1 = pygame.font.Font('freesansbold.ttf', 14)
FONT_2 = pygame.font.Font('freesansbold.ttf', 32)

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

