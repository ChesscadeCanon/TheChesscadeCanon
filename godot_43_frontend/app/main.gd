extends Control

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

@onready var title := $VBoxContainer/ButtonHBox/CenterContainer/Title
@onready var title_animator := $VBoxContainer/ButtonHBox/CenterContainer/Title/AnimationPlayer
@onready var instructions := $VBoxContainer/ButtonHBox/CenterContainer/Instructions
@onready var back_button := $VBoxContainer/ButtonHBox/BackButton
@onready var play_pause_button := $VBoxContainer/ButtonHBox/PlayPauseButton
@onready var time := $VBoxContainer/ReadoutHBox/Time
@onready var score := $VBoxContainer/ReadoutHBox/Score
@onready var countdown := $VBoxContainer/BoardCenter/BoardAspectRatio/Countdown
@onready var header := $VBoxContainer/DeckCenter/DeckAspectRatio/Header

func set_state(to: State)->void:
	emit_signal("state_changed", state, to)
	last_state = state
	state = to

# Called when the node enters the scene tree for the first time.
func _ready():
	set_state(State.TITLE)
	get_tree().get_root().size_changed.connect(_resize)

func _resize()->void:
	var target := Vector2i(
		ProjectSettings.get_setting("display/window/size/viewport_width"),
		ProjectSettings.get_setting("display/window/size/viewport_width")
	)
	var actual := get_viewport_rect().size
	var loss := target.y - actual.y
	var deck_aspect := $VBoxContainer/DeckCenter/DeckAspectRatio
	var board_aspect := $VBoxContainer/BoardCenter/BoardAspectRatio
	var deck := $VBoxContainer/DeckCenter/DeckAspectRatio/Deck
	var board := $VBoxContainer/BoardCenter/BoardAspectRatio/Board
	var countdown := $VBoxContainer/BoardCenter/BoardAspectRatio/Countdown
	custom_minimum_size.y = actual.y
	size.y = actual.y
	$VBoxContainer.custom_minimum_size.y = actual.y

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

func get_high_score()->int:
	var ret := 0
	if FileAccess.file_exists("user://high_score.dat"):
		var in_file := FileAccess.open("user://high_score.dat", FileAccess.READ)
		var text := in_file.get_line()
		ret = int(text)
		in_file.close()
	return ret

func submit_score()->void:
	var score := ChesscadeModel.get_score()
	var high_score := get_high_score()
	if score > high_score:
		var out_file := FileAccess.open("user://high_score.dat", FileAccess.WRITE)
		out_file.resize(0)
		out_file.store_string(str(score))
		out_file.close()

func _pause()->void:
	set_process(false)
	ChesscadeModel.process_mode = Node.PROCESS_MODE_DISABLED
	ChesscadeModel.pause()
	
func _unpause()->void:
	set_process(true)
	ChesscadeModel.process_mode = Node.PROCESS_MODE_ALWAYS
	ChesscadeModel.unpause()
	
func _on_state_changed(from: State, to: State):
	countdown.stop()
	countdown.visible = to == State.COUNTDOWN
	set_process_input(to == State.PLAY)
	
	if from == State.PLAY:
		submit_score()

	match to:
		State.TITLE:
			_pause()
			ChesscadeModel.reset()
			title_animator.stop()
			title_animator.play('pulse')
			title.visible = true
			title.text = 'Chesscade'
			instructions.visible = false
			_play_pause_be_play()
			_back_be_quit()
			score.show_high_score(get_high_score())
		State.COUNTDOWN:
			_pause()
			title_animator.stop()
			title_animator.play('flash')
			title.visible = true
			title.text = 'Get Ready'
			instructions.visible = false
			countdown.play('default')
			_play_pause_be_pause()
			_back_be_back()
		State.PLAY:
			_unpause()
			title_animator.stop()
			title.visible = false
			instructions.visible = true
			if ChesscadeModel.get_time() == 0.0:
				ChesscadeModel.begin_game()
			_play_pause_be_pause()
			_back_be_back()
		State.PAUSE:
			_pause()
			title_animator.stop()
			title_animator.play('pulse')
			title.visible = true
			title.text = 'Paused'
			instructions.visible = false
			_play_pause_be_play()
			_back_be_back()
		State.OVER:
			_pause()
			title_animator.stop()
			title_animator.play('pulse')
			title.visible = true
			title.text = 'Game Over'
			instructions.visible = false
			_play_pause_be_play()
			_back_be_back()
			
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

func _on_rules_button_pressed() -> void:
	$Rules.visible = true
	$Rules/ScrollContainer.grab_focus.call_deferred()
	
	if state in [State.PLAY, State.COUNTDOWN]:
		state = State.PAUSE
