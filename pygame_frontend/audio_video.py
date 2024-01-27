from re import L
import pygame
from os import environ
import style

pygame.init()

if "REPL_OWNER" not in environ:
    import pygame.midi
    pygame.midi.init()
    from instrument import MIDINote

    def prepare_notes(volume, layers, low_note, high_note, step):
        port = pygame.midi.get_default_output_id()
        midi_out = pygame.midi.Output(port, 0)
        notes = []
        for grade in range(layers):
            notes.append(list(range(low_note + step * grade, low_note + step * (grade+1), 2)))
        print(notes)
        port = pygame.midi.get_default_output_id()

        adjust_volume = lambda v, n: v - (n - low_note) // 2
        return [[MIDINote(midi_out, n, adjust_volume(volume, n)) for n in s] for s in notes]
    fall_notes = prepare_notes(75, 4, 16, 96, 16)
    
FONT_0 = pygame.font.Font('freesansbold.ttf', 11)
FONT_1 = pygame.font.Font('freesansbold.ttf', 14)
FONT_2 = pygame.font.Font('freesansbold.ttf', 32)

PIECES = ['p', 'q', 'k', 'n', 'b', 'r']
piece_path = lambda p, d: (d + ('w' + p if p.isupper() else 'b' + p)).lower() + ".png"
load_piece = lambda n, d: pygame.image.load(piece_path(n, d))
scale_piece = lambda p: pygame.transform.scale(p, (style.SQUARE, style.SQUARE))
setup_piece = lambda n, d: scale_piece(load_piece(n, d))

def tint_piece(p, n):
    pixels = pygame.PixelArray(p)
    pixels.replace(style.BLACK if n.isupper() else style.WHITE, style.GREY)
    del pixels
    return p

def load_pieces(directory):
    ret = {}
    [ret.update({p: setup_piece(p, directory)}) for p in PIECES + list([w.upper() for w in PIECES])]
    return ret

board_pieces = load_pieces(style.BOARD_PIECES_DIR)
deck_pieces = load_pieces(style.DECK_PIECES_DIR)
next_pieces = load_pieces(style.NEXT_PIECES_DIR)
threatened_pieces = load_pieces(style.THREATENED_PIECES_DIR)
shadow_pieces = load_pieces(style.SHADOW_PIECES_DIR)
player_pieces = load_pieces(style.PLAYER_PIECES_DIR)

