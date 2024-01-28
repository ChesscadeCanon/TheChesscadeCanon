import pygame
import style

FONT_0 = pygame.font.Font('freesansbold.ttf', 11)
FONT_1 = pygame.font.Font('freesansbold.ttf', 14)
FONT_2 = pygame.font.Font('freesansbold.ttf', 32)

PIECES = ['p', 'q', 'k', 'n', 'b', 'r']
piece_path = lambda piece, directory: (directory + ('w' + piece if piece.isupper() else 'b' + piece)).lower() + ".png"
load_piece = lambda piece_name, directory: pygame.image.load(piece_path(piece_name, directory))
scale_piece = lambda piece: pygame.transform.scale(piece, (style.SQUARE, style.SQUARE))
setup_piece = lambda piece_name, d: scale_piece(load_piece(piece_name, d))

def load_pieces(directory):
    ret = {}
    [ret.update({piece: setup_piece(piece, directory)}) for piece in PIECES + list([white_piece.upper() for white_piece in PIECES])]
    return ret

board_pieces = load_pieces(style.BOARD_PIECES_DIR)
deck_pieces = load_pieces(style.DECK_PIECES_DIR)
next_pieces = load_pieces(style.NEXT_PIECES_DIR)
threatened_pieces = load_pieces(style.THREATENED_PIECES_DIR)
shadow_pieces = load_pieces(style.SHADOW_PIECES_DIR)
player_pieces = load_pieces(style.PLAYER_PIECES_DIR)
arrow = pygame.image.load(style.ARROW_PNG)
