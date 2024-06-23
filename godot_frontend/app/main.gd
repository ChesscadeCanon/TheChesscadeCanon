extends VBoxContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	ChesscadeModel.begin_game()
	ChesscadeModel.process_mode = Node.PROCESS_MODE_INHERIT
	$ReadoutCenter/ReadoutHBox/Time.show_time()
	$ReadoutCenter/ReadoutHBox/Score.show_score()

func _process(delta)->void:
	var left := Input.is_action_pressed("ui_left")
	var right := Input.is_action_pressed("ui_right")
	var down := Input.is_action_pressed("ui_down")
	var drop := Input.is_action_just_pressed("ui_select")
	ChesscadeModel.input_digital_move(left, right, down)
	if drop:
		ChesscadeModel.input_drop()
	$ReadoutCenter/ReadoutHBox/Time.show_time()
	$ReadoutCenter/ReadoutHBox/Score.show_score()
