import ctypes
import pygame
from res import *
from os import environ

history = set()

pygame.init()
high_score = 0
def update_high_score(score):
    global high_score
    with open('high_score.txt', 'r') as file:
        line = file.readline()
        high_score = int(line) if line else high_score
    with open('high_score.txt', 'w') as file:
        if score > high_score:
            high_score = score
        file.write(str(high_score))
    return high_score

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

RULES = ''.join([chr(b) for b in engine.get_rules()])
BLACK = ( 0, 0, 0)
WHITE = ( 255, 255, 255)
GREY = ( 127, 127, 127)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
DARK_GREY = (63, 63, 63)
LIGHT_GREY = (171, 171, 171)
DARK_ORANGE = (255, 127, 80)
LIGHT_ORANGE = (255, 215, 0)
DARK_BLUE = (0, 0, 64)
LIGHT_BLUE = (191, 191, 255)
DARK_RED = (64, 0, 0)
LIGHT_RED = (255, 191, 191)
SIZE = (400, 480)
RANKS = engine.get_ranks()
FILES = engine.get_files()
SYMBOL_COUNT = engine.get_symbol_count()
BOARD_LENGTH = engine.get_board_length()
SQUARE = 40
SQUARES_OFF_LEFT = 4
SQUARES_OFF_TOP = 4
SIZE = (FILES + SQUARES_OFF_LEFT) * SQUARE, (RANKS + SQUARES_OFF_TOP) * SQUARE
SQUARE = min(SIZE[0] // (FILES + SQUARES_OFF_LEFT), SIZE[1] // (RANKS + SQUARES_OFF_TOP))
FONT_0 = pygame.font.Font('freesansbold.ttf', 11)
FONT_1 = pygame.font.Font('freesansbold.ttf', 14)
FONT_2 = pygame.font.Font('freesansbold.ttf', 32)

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
    white = WHITE
    black = BLACK
    pattern = engine.attack_pattern(game)
    if engine.is_on_brink(game):
        white = LIGHT_ORANGE
        black = DARK_ORANGE
    elif game.contents.repeat:
        white = LIGHT_BLUE
        black = DARK_BLUE
    color = white
    for f in range(FILES):
        for r in range(RANKS):
            bit = engine.get_square_bit(r, f)
            x = (SQUARES_OFF_LEFT + f) * SQUARE
            y = (SQUARES_OFF_TOP + r) * SQUARE
            pattern_color = LIGHT_RED if color == white else DARK_RED
            in_pattern = bit & pattern
            final_color = pattern_color if in_pattern else color
            pygame.draw.rect(screen, final_color, [x, y, SQUARE, SQUARE], 0)
            color = black if color == white else white
        color = black if color == white else white

def get_board(game):
    return ''.join([chr(c) for c in game.contents.board])

def draw_pieces(game):
    index = 0
    threats = engine.forecast_captures(game)
    board = get_board(game)
    if game.contents.repeat:
        if board in history:
            print("Yup, that's a repeat!")
        else:
            print("False positive!")
    history.add(board)
    for r in range(RANKS):
        for f in range(FILES):
            while board[index] == '\n':
                index += 1
            square = board[index]
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
    cursor_grade = game.contents.cursor_grade
    cursor_increment = game.contents.cursor_increment
    next_piece = engine.get_next_piece(game)
    for g in range(SQUARES_OFF_TOP):
        deck = [chr(c) for c in engine.get_deck(g)]
        draw_piece_on_deck(deck_pieces, deck[0], g, 0)
        [draw_piece_on_deck(deck_pieces, deck[i], g, i) for i in range(FILES)]
    draw_piece_on_deck(next_pieces, next_piece, cursor_grade, cursor_increment)

def draw_shadow(game):
    piece = engine.get_forecast_piece(game)
    rank = engine.get_forecast_rank(game)
    file = game.contents.player_file
    draw_piece_on_square(shadow_pieces, piece, rank, file)

def draw_text(game):
    readouts = ("Score", f"{game.contents.score}"), ("Scored", f"{game.contents.scored}"), ("Combo", f"{game.contents.combo}"), ("Delay", f"{engine.get_ease(game)}")
    y = 0
    for readout in readouts:
        for part in readout:    
            text = FONT_1.render(part, True, WHITE, BLACK)
            rect = text.get_rect()
            rect.topright = (SQUARE * SQUARES_OFF_LEFT, y)
            screen.blit(text, rect)
            y += text.get_rect().height
        y += text.get_rect().height
    controls = [
        ("arrow keys: move"), 
        ("left click and drag:"), 
        ("    move"),
        ("space/right_click: drop"), 
        ("p: pause/help"), 
        ("backspace: back"),
        ("q: quit"),
        ]
    for control in controls:
        text = FONT_1.render(control, True, BLACK, WHITE)
        rect = text.get_rect()
        rect.topleft = (0, y)
        screen.blit(text, rect)
        y += text.get_rect().height
    warnings = ((game.contents.repeat, "REPEAT!", BLUE, WHITE), (engine.is_on_brink(game), "WATCH OUT!", RED, BLACK))
    for warning in warnings:
        if warning[0]:
            text = FONT_1.render(warning[1], True, warning[2], warning[3])
            rect = text.get_rect()
            y += text.get_rect().height
            rect.center = (SQUARE * SQUARES_OFF_LEFT * 0.5, y)
            screen.blit(text, rect)

def play_sounds(game, passed):
    
    if "REPL_OWNER" not in environ:
        if game.contents.events & EVENT_FELL:
            ease = maxtime=engine.get_ease(game)
            note = fall_notes[game.contents.cursor_grade][game.contents.cursor_increment]
            note.play(ease)
    
        [[n.pump(passed) for n in s] for s in fall_notes]

def take_input(game, passed):
    keys=pygame.key.get_pressed()
    engine.input_digital_move(game, keys[pygame.K_LEFT], keys[pygame.K_RIGHT], keys[pygame.K_DOWN])
    if pygame.mouse.get_focused() and pygame.mouse.get_pressed(3)[0]:
        rel = pygame.mouse.get_rel()
        x = rel[0] / SQUARE
        y = rel[1] / SQUARE
        engine.input_analog_move(game, x, y)

def draw_title():
    global high_score
    title_label = FONT_2.render("Chesscade", True, BLACK, WHITE)
    title_rect = title_label.get_rect()
    title_rect.center = SIZE[0] / 2, SIZE[1] / 4
    screen.blit(title_label, title_rect)
    score_label = FONT_1.render(f"High Score: {str(high_score)}", True, BLACK)
    score_rect = score_label.get_rect()
    score_rect.center = SIZE[0] / 2, SIZE[1] / 3
    screen.blit(score_label, score_rect)
    help_label = FONT_1.render("press ENTER to play, h for help, or q to quit", True, BLACK)
    help_rect = help_label.get_rect()
    help_rect.center = SIZE[0] / 2, SIZE[1] / 2
    screen.blit(help_label, help_rect)

def draw_help():
    lines = ["arrow keys or left click and drag to move, space or right click to drop"]
    lines += ["p to pause, q to quit, backspace to go back", ""]
    lines += RULES.split('\n')
    x = SIZE[0] / 2
    y = SIZE[1] / 4
    for line in lines:
        label = FONT_0.render(line, True, BLACK)
        rect = label.get_rect()
        rect.center = x, y
        screen.blit(label, rect)
        y += rect.height

def title():
    carryOn = True
    helping = False
    clock = pygame.time.Clock()
    while True:
        for event in pygame.event.get():     
            if event.type == pygame.QUIT: 
                return 0
            elif event.type == pygame.KEYDOWN:
                if helping:
                    helping = False
                elif event.key == pygame.K_q:
                    return 0
                elif event.key in (pygame.K_RETURN, pygame.K_KP_ENTER):
                    return 1
                elif event.key == pygame.K_h:
                    helping = True
        screen.fill(GREY)
        if helping:
            label = FONT_0.render("press any key to return to main screen", True, BLACK)
            rect = label.get_rect()
            rect.center = SIZE[0] / 2, SIZE[1] / 8
            screen.blit(label, rect)
            draw_help()
        else:
            draw_title()
        passed = clock.tick(30)
        pygame.display.flip()

def draw_game(game):
    draw_board(game)
    draw_next(game)
    draw_pieces(game)
    draw_shadow(game)
    draw_player(game)
    draw_text(game)

def draw_game_over(game):
    if engine.is_game_over(game):    
        title_label = FONT_2.render("Game Over", True, WHITE, BLACK)
        title_rect = title_label.get_rect()
        title_rect.center = SIZE[0] / 2, SIZE[1] / 2
        screen.blit(title_label, title_rect)

GO = 0
BACK = 1
QUIT = 2

def play():
    ret = 0
    game = engine.malloc_init_default_game()
    engine.begin_game(game)
    go = GO
    over = False
    back = False
    clock = pygame.time.Clock()
    while go == GO:
        for event in pygame.event.get(): 
            if event.type == pygame.QUIT: 
                go = QUIT 
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_BACKSPACE:
                    go = BACK
                elif event.key == pygame.K_SPACE:
                    engine.input_drop(game)
                elif event.key == pygame.K_p:
                    engine.input_toggle_pause(game)
                elif event.key == pygame.K_q:
                    go = QUIT
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 3:
                    if pygame.mouse.get_focused():
                        engine.input_drop(game)
        passed = clock.tick(30)
        take_input(game, passed)
        if over:
            pass
        else:
            engine.pump_game(game, passed)
        over = engine.is_game_over(game)
        screen.fill(GREY)
        if engine.is_paused(game):
            label = FONT_2.render("paused", True, BLACK)
            rect = label.get_rect()
            rect.center = SIZE[0] / 2, SIZE[1] / 8
            screen.blit(label, rect)
            draw_help()
        else:
            draw_game(game)
        play_sounds(game, passed)
        draw_game_over(game)
        pygame.display.flip()
        clock.tick(60)
    update_high_score(game.contents.score)
    over = engine.is_game_over(game)
    engine.delete_game(game)
    return go

while True:
    update_high_score(high_score)
    do = title()
    if do > 0:
        do = play()
        if do == QUIT:
            break
    else:
        break

pygame.midi.quit()
pygame.quit()

    