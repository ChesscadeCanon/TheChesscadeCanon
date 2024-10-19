extends VBoxContainer

const PLAY := preload('res://assets/play.png')
const PLAY_SELECTED := preload('res://assets/play.png')
const PAUSE := preload('res://assets/pause.png')
const PAUSE_SELECTED := preload('res://assets/pause.png')

enum State {
	TITLE,
	COUNTDOWN,
	PLAY,
	PAUSE,
	OVER,
	CONFIRM
}

signal state_changed(from: State, to: State)

var state :State= State.TITLE: set = set_state
var last_state :State= State.TITLE

@onready var back_button := $ButtonCenter/ButtonHBox/BackButton
@onready var play_pause_button := $ButtonCenter/ButtonHBox/PlayPauseButton
@onready var time := $ReadoutCenter/ReadoutHBox/Time
@onready var score := $ReadoutCenter/ReadoutHBox/Score
@onready var countdown := $BoardCenter/BoardAspectRatio/Countdown
@onready var header := $DeckCenter/DeckAspectRatio/Header
@onready var title_menu := $BoardCenter/BoardAspectRatio/TitleMenu
@onready var quit_menu := $BoardCenter/BoardAspectRatio/QuitMenu
@onready var pause_menu := $BoardCenter/BoardAspectRatio/PauseMenu

func set_state(to: State)->void:
	emit_signal("state_changed", state, to)
	last_state = state
	state = to

# Called when the node enters the scene tree for the first time.
func _ready():
	set_state(State.TITLE)
	time.show_time()
	score.show_score()

func _process_input(delta)->void:
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
	header.visible = to != State.PLAY
	title_menu.visible = to == State.TITLE
	quit_menu.visible = to == State.CONFIRM
	pause_menu.visible = to == State.PAUSE
	back_button.disabled = to == State.CONFIRM
	
	set_process_input(to == State.PLAY)

	match to:
		State.TITLE:
			header.text = 'Chesscade'
			ChesscadeModel.reset()
			_play_pause_be_play()
		State.COUNTDOWN:
			header.text = 'Get\nReady!'
			countdown.play('default')
			_play_pause_be_pause()
		State.PLAY:
			if ChesscadeModel.get_time() == 0.0:
				ChesscadeModel.begin_game()
			_play_pause_be_pause()
		State.PAUSE:
			header.text = 'Paused'
			_play_pause_be_play()
		State.OVER:
			header.text = 'Game\nOver'
			_play_pause_be_play()
		State.CONFIRM:
			header.text = 'Quit\nGame?' if from == State.TITLE else 'Main\nMenu?'
			_play_pause_be_play()

	if to == State.PLAY:
		ChesscadeModel.process_mode = Node.PROCESS_MODE_ALWAYS
		ChesscadeModel.unpause()
	else:
		ChesscadeModel.process_mode = Node.PROCESS_MODE_DISABLED
		ChesscadeModel.pause()

func _cancel_quit()->void:
	match state:
		State.CONFIRM:
			state = last_state if last_state != State.PLAY else State.COUNTDOWN
		_:
			breakpoint

func _on_play_pause_button_pressed()->void:
	play_pause_button.release_focus()
	match state:
		State.CONFIRM:
			_cancel_quit()
			return
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

func _on_back_button_pressed():
	back_button.release_focus()
	state = State.CONFIRM

func _on_quit_button_pressed():
	match last_state:
		State.TITLE:
			get_tree().quit()
		_:
			state = State.TITLE

func _on_cancel_button_pressed():
	_cancel_quit()

func _on_resume_button_pressed():
	state = State.COUNTDOWN
