import librarian
import style
import audio
from audio import init_notes, get_notes, quit_audio, stop_notes, pump_notes
import pygame
pygame.init()
from video import *

high_score = 0
def update_high_score(score):
    global high_score
    with open('high_score.txt', 'w+') as file:
        pass
    with open('high_score.txt', 'r') as file:
        line = file.readline()
        high_score = int(line) if line else high_score
    if score > high_score:
        high_score = score
    with open('high_score.txt', 'w') as file:
        file.write(str(high_score))
    return high_score

engine = librarian.Engine()
init_notes()

RULES = ''.join([chr(b) for b in engine.get_rules()])
RANKS = engine.get_ranks()
FILES = engine.get_files()
SIZE = (FILES + style.SQUARES_OFF_LEFT) * style.SQUARE, (RANKS + style.SQUARES_OFF_TOP) * style.SQUARE

screen = pygame.display.set_mode(SIZE)
pygame.display.set_caption("Chesscade")

square_to_point = lambda rank, file: ((file + style.SQUARES_OFF_TOP) * style.SQUARE, (rank + style.SQUARES_OFF_LEFT) * style.SQUARE)
draw_on_square = lambda image, rank, file: screen.blit(image, square_to_point(rank, file))
draw_piece = lambda pieces, piece, x, y: piece in pieces and screen.blit(pieces[piece], (x, y))
draw_piece_on_square = lambda pieces, piece, r, f: draw_piece(pieces, piece, (f + style.SQUARES_OFF_LEFT) * style.SQUARE, (r + style.SQUARES_OFF_TOP) * style.SQUARE)
draw_piece_on_deck = lambda pieces, piece, r, f: draw_piece(pieces, piece, (f + style.SQUARES_OFF_LEFT) * style.SQUARE, r * style.SQUARE)

def draw_cursor(game):
    direction = engine.get_cursor_direction(game)
    arrow = right_arrow if direction > 0 else left_arrow
    spawn_rank = engine.get_spawn_rank(game)
    spawn_file = engine.get_cursor_increment(game)
    draw_on_square(arrow, spawn_rank, spawn_file)

def draw_board(game):
    white = style.WHITE
    black = style.BLACK
    pattern = engine.attack_pattern(game)
    if engine.is_on_brink(game):
        white = style.LIGHT_ORANGE
        black = style.DARK_ORANGE
    elif engine.is_repeat(game):
        white = style.LIGHT_BLUE
        black = style.DARK_BLUE
    color = white
    for f in range(FILES):
        for r in range(RANKS):
            bit = engine.get_square_bit(r, f)
            x = (style.SQUARES_OFF_LEFT + f) * style.SQUARE
            y = (style.SQUARES_OFF_TOP + r) * style.SQUARE
            pattern_color = style.LIGHT_RED if color == white else style.DARK_RED
            in_pattern = bit & pattern
            final_color = pattern_color if in_pattern else color
            pygame.draw.rect(screen, final_color, [x, y, style.SQUARE, style.SQUARE], 0)
            color = black if color == white else white
        color = black if color == white else white

def draw_pieces(game):
    index = 0
    threats = engine.forecast_captures(game)
    for r in range(RANKS):
        for f in range(FILES):
            square = engine.get_square(game, r, f).decode('ascii')
            if square in board_pieces:
                if threats & engine.get_square_bit(r, f):
                    draw_piece_on_square(threatened_pieces, square, r, f)
                else:
                    draw_piece_on_square(board_pieces, square, r, f)
            index += 1

def draw_player(game):
    player = engine.get_player(game).decode('ascii')
    rank = engine.get_player_rank(game)
    file = engine.get_player_file(game)
    draw_piece_on_square(player_pieces, player, rank, file)

def draw_next(game):
    cursor_grade = engine.get_cursor_grade(game)
    cursor_increment = engine.get_cursor_increment(game)
    next_piece = engine.get_next_piece(game).decode('ascii')
    for g in range(style.SQUARES_OFF_TOP):
        for i in range(FILES):
            piece = engine.get_deck_piece(game, g, i).decode('ascii')
            draw_piece_on_deck(deck_pieces, piece, g, i)
    draw_piece_on_deck(next_pieces, next_piece, cursor_grade, cursor_increment)

def draw_shadow(game):
    piece = engine.get_forecast_piece(game).decode('ascii')
    rank = engine.get_forecast_rank(game)
    file = engine.get_player_file(game)
    draw_piece_on_square(shadow_pieces, piece, rank, file)

def draw_text(game):
    readouts = ("Score", f"{engine.get_score(game)}"), ("Scored", f"{engine.get_scored(game)}"), ("Time", f"{engine.get_milliseconds(game)}")
    y = 0
    for readout in readouts:
        for part in readout:    
            text = FONT_1.render(part, True, style.WHITE, style.BLACK)
            rect = text.get_rect()
            rect.topright = (style.SQUARE * style.SQUARES_OFF_LEFT, y)
            screen.blit(text, rect)
            y += text.get_rect().height
        y += text.get_rect().height
    controls = (
        ("arrow keys: move"), 
        ("left click + drag: move"), 
        ("space/right click: drop"), 
        ("p: pause/help"), 
        ("backspace: back"),
        ("q: quit"),
        )
    for control in controls:
        text = FONT_1.render(control, True, style.BLACK, style.WHITE)
        rect = text.get_rect()
        rect.topleft = (0, y)
        screen.blit(text, rect)
        y += text.get_rect().height
    if not engine.is_game_over(game):
        warnings = ((engine.is_repeat(game), "REPEAT!", style.BLUE, style.WHITE), (engine.is_on_brink(game), "WATCH OUT!", style.RED, style.BLACK))
        for warning in warnings:
            if warning[0]:
                text = FONT_1.render(warning[1], True, warning[2], warning[3])
                rect = text.get_rect()
                y += text.get_rect().height
                rect.center = (style.SQUARE * style.SQUARES_OFF_LEFT * 0.5, y)
                screen.blit(text, rect)

def play_sounds(game):
    notes = get_notes()
    if engine.is_game_over(game):
        stop_notes()
        return;
    if notes:
        if engine.get_events(game) & (librarian.EVENT_LEFT | librarian.EVENT_RIGHT | librarian.EVENT_DOWN):
            notes.move_note.play()
        if engine.get_events(game) & librarian.EVENT_WRAPPED:
            notes.wrap_note.play()
        elif engine.get_events(game) & librarian.EVENT_FELL:
            note = notes.fall_notes[engine.get_cursor_grade(game)][engine.get_cursor_increment(game)]
            note.play()
        if engine.get_events(game) & librarian.EVENT_CAPTURED:
            notes.capture_note.play()
        elif engine.get_events(game) & librarian.EVENT_DROPPED:
            notes.drop_note.play()
        elif engine.get_events(game) & librarian.EVENT_LANDED:
            notes.land_note.play()

def take_input(game, passed):
    keys=pygame.key.get_pressed()
    engine.input_digital_move(game, keys[pygame.K_LEFT], keys[pygame.K_RIGHT], keys[pygame.K_DOWN])
    if pygame.mouse.get_focused() and pygame.mouse.get_pressed(3)[0]:
        rel = pygame.mouse.get_rel()
        x = rel[0] / style.SQUARE
        y = rel[1] / style.SQUARE
        engine.input_analog_move(game, x, y)

def draw_title():
    global high_score
    title_label = FONT_2.render("Chesscade", True, style.BLACK, style.WHITE)
    title_rect = title_label.get_rect()
    title_rect.center = SIZE[0] / 2, SIZE[1] / 4
    screen.blit(title_label, title_rect)
    score_label = FONT_1.render(f"High Score: {str(high_score)}", True, style.BLACK)
    score_rect = score_label.get_rect()
    score_rect.center = SIZE[0] / 2, SIZE[1] / 3
    screen.blit(score_label, score_rect)
    help_label = FONT_1.render("press ENTER to play, h for help, or q to quit", True, style.BLACK)
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
        label = FONT_0.render(line, True, style.BLACK)
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
        screen.fill(style.GREY)
        if helping:
            label = FONT_0.render("press any key to return to main screen", True, style.BLACK)
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
    draw_cursor(game)
    draw_pieces(game)
    draw_shadow(game)
    draw_player(game)
    draw_text(game)

def draw_game_over(game):
    if engine.is_game_over(game):    
        title_label = FONT_2.render("Game Over", True, style.WHITE, style.BLACK)
        title_rect = title_label.get_rect()
        title_rect.center = SIZE[0] / 2, SIZE[1] / 2
        screen.blit(title_label, title_rect)

def draw_pause(game):
    label = FONT_2.render("paused", True, style.BLACK)
    rect = label.get_rect()
    rect.center = SIZE[0] / 2, SIZE[1] / 8
    screen.blit(label, rect)
    draw_help()

GO = 0
BACK = 1
QUIT = 2

def play_events(game):
    ret = GO
    for event in pygame.event.get(): 
        if event.type == pygame.QUIT: 
            ret = QUIT 
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_BACKSPACE:
                ret = BACK
            elif event.key == pygame.K_SPACE:
                engine.input_drop(game)
            elif event.key == pygame.K_p:
                engine.input_toggle_pause(game)
            elif event.key == pygame.K_q:
                ret = QUIT
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 3:
                if pygame.mouse.get_focused():
                    engine.input_drop(game)
    return ret

def play_game(game, passed):
    take_input(game, passed)
    if engine.is_game_over(game):
        stop_notes()
        return True
    else:
        engine.pump_game(game, passed)
        pump_notes(passed)
    return False

def play_draw(game,):
    screen.fill(style.GREY)
    if engine.is_paused(game):
        draw_pause(game)
        stop_notes()
    else:
        draw_game(game)
    draw_game_over(game)
    pygame.display.flip()

def play():
    ret = 0
    game = engine.malloc_init_default_game()
    engine.begin_game(game)
    go = GO
    over = False
    back = False
    clock = pygame.time.Clock()
    while go == GO:
        passed = clock.tick(60)
        go = play_events(game)
        over = play_game(game, passed)
        play_draw(game)
        play_sounds(game)
    update_high_score(engine.get_score(game))
    over = engine.is_game_over(game)
    engine.delete_game(game)
    return go

while True:
    update_high_score(high_score)
    do = title()
    if do > 0:
        do = play()
        stop_notes()
        if do == QUIT:
            break
    else:
        break

quit_audio()
pygame.quit()

    