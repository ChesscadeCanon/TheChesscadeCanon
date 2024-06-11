from re import L
import pygame
from os import environ

from pygame import midi
import style

pygame.init()

if "REPL_OWNER" not in environ:
    import pygame.midi
    pygame.midi.init()
    from instrument import MIDINote
    port = pygame.midi.get_default_output_id()
    midi_out = pygame.midi.Output(port, 0)
    def prepare_notes(instrument, volume=0, layers=0, low_note=0, outer_step=0, inner_step=0):
        notes = []
        for layer in range(layers):
            notes.append(list(range(low_note + outer_step * layer, low_note + outer_step * (layer+1), inner_step)))
        adjust_volume = lambda v, n: v - (n - low_note) // 2
        return [[MIDINote(midi_out, instrument=instrument, note=n, volume=volume, length=64 * 36) for n in s] for s in notes]
    fall_notes = prepare_notes(instrument=114, volume=64, layers=4, low_note=48, outer_step=16, inner_step=2)
    move_note = MIDINote(midi_out, instrument=116, note=96, volume=32, length=64)
    drop_note = MIDINote(midi_out, instrument=118, note=32, volume=96, length=128)
    land_note = MIDINote(midi_out, instrument=113, note=48, volume=64, length=128)
    capture_note = MIDINote(midi_out, instrument=108, note=64, volume=72, length=512)
    wrap_note = MIDINote(midi_out, instrument=57, note=64, volume=64, length=256)
    
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

