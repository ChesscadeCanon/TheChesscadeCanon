extends VBoxContainer

const PLAY := preload('res://assets/play.png')
const PLAY_SELECTED := preload('res://assets/play_selected.png')
const PAUSE := preload('res://assets/pause.png')
const PAUSE_SELECTED := preload('res://assets/pause_selected.png')
const BACK := preload('res://assets/back.png')
const BACK_SELECTED := preload('res://assets/back_selected.png')
const QUIT := preload('res://assets/X.png')
const QUIT_SELECTED := preload('res://assets/X_selected.png')

enum State {
	TITLE,
	COUNTDOWN,
	PLAY,
	PAUSE,
	OVER
}

signal state_changed(from: State, to: State)

var state :State= State.TITLE: set = set_state
var last_state :State= State.TITLE

@onready var back_button := $ButtonHBox/BackButton
@onready var play_pause_button := $ButtonHBox/PlayPauseButton
@onready var time := $ReadoutHBox/Time
@onready var score := $ReadoutHBox/Score
@onready var countdown := $BoardCenter/BoardAspectRatio/Countdown
@onready var header := $DeckCenter/DeckAspectRatio/Header

func set_state(to: State)->void:
	emit_signal("state_changed", state, to)
	last_state = state
	state = to

# Called when the node enters the scene tree for the first time.
func _ready():
	set_state(State.TITLE)
	time.show_time()
	score.show_score()

func _process(delta)->void:
	if ChesscadeModel.is_game_over():
		state = State.OVER
		return
	
	var left := Input.is_action_pressed("ui_left")
	var right := Input.is_action_pressed("ui_right")
	var down := Input.is_action_pressed("ui_down")
	var drop := Input.is_action_just_pressed("ui_select")
	ChesscadeModel.input_digital_move(left, right, down)
	
	if drop:
		ChesscadeModel.input_drop()
		
	time.show_time()
	score.show_score()

func _on_state_changed(from: State, to: State):
	countdown.stop()
	countdown.visible = to == State.COUNTDOWN
	
	set_process_input(to == State.PLAY)

	match to:
		State.TITLE:
			ChesscadeModel.reset()
			_play_pause_be_play()
			_back_be_quit()
		State.COUNTDOWN:
			countdown.play('default')
			_play_pause_be_pause()
			_back_be_back()
		State.PLAY:
			if ChesscadeModel.get_time() == 0.0:
				ChesscadeModel.begin_game()
			_play_pause_be_pause()
			_back_be_back()
		State.PAUSE:
			_play_pause_be_play()
			_back_be_back()
		State.OVER:
			_play_pause_be_play()
			_back_be_back()

	if to in [State.PLAY, State.OVER]:
		ChesscadeModel.process_mode = Node.PROCESS_MODE_ALWAYS
		ChesscadeModel.unpause()
	else:
		ChesscadeModel.process_mode = Node.PROCESS_MODE_DISABLED
		ChesscadeModel.pause()

func _on_play_pause_button_pressed()->void:
	play_pause_button.release_focus()
	match state:
		State.COUNTDOWN, State.PLAY:
			state = State.PAUSE
			return
		State.OVER:
			ChesscadeModel.reset()
	state = State.COUNTDOWN
			
func _on_countdown_animation_finished():
	state = State.PLAY

func _play_pause_be_play()->void:
	play_pause_button.texture_normal = PLAY
	play_pause_button.texture_pressed = PLAY_SELECTED
	
func _play_pause_be_pause()->void:
	play_pause_button.texture_normal = PAUSE_SELECTED
	play_pause_button.texture_pressed = PAUSE
	
func _back_be_back()->void:
	back_button.texture_normal = BACK
	back_button.texture_pressed = BACK_SELECTED
	
func _back_be_quit()->void:
	back_button.texture_normal = QUIT
	back_button.texture_pressed = QUIT_SELECTED

func _on_back_button_pressed():
	back_button.release_focus()
	match state:
		State.TITLE:
			get_tree().quit()
		State.OVER:
			state = State.TITLE
		State.PLAY, State.COUNTDOWN:
			state = State.PAUSE
		_:
			state = State.TITLE
