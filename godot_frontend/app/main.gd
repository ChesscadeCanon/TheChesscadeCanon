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

@onready var back_button := $ButtonCenter/ButtonHBox/BackButton
@onready var play_pause_button := $ButtonCenter/ButtonHBox/PlayPauseButton
@onready var time := $ButtonCenter/ButtonHBox/Time
@onready var score := $ButtonCenter/ButtonHBox/Score
@onready var countdown := $BoardCenter/BoardAspectRatio/Countdown

func set_state(to: State)->void:
	emit_signal("state_changed", state, to)
	state = to

# Called when the node enters the scene tree for the first time.
func _ready():
	set_state(State.TITLE)
	time.show_time()
	score.show_score()

func _process(delta)->void:
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
	match to:
		State.TITLE:
			countdown.visible = false
			ChesscadeModel.reset()
			ChesscadeModel.pause()
		State.COUNTDOWN:
			countdown.visible = true
			countdown.play('default')
			ChesscadeModel.pause()
		State.PLAY:
			countdown.visible = false
			if ChesscadeModel.get_time() == 0.0:
				ChesscadeModel.begin_game()
			ChesscadeModel.process_mode = Node.PROCESS_MODE_INHERIT
			ChesscadeModel.unpause()
		State.PAUSE:
			countdown.visible = false
			ChesscadeModel.pause()
		State.OVER:
			countdown.visible = false
			ChesscadeModel.pause()
		State.CONFIRM:
			countdown.visible = false
			ChesscadeModel.pause()

func _on_play_pause_button_pressed():
	match state:
		State.TITLE:
			state = State.COUNTDOWN
			
func _on_countdown_animation_finished():
	state = State.PLAY
