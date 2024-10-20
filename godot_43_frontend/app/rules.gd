extends Button

@onready var body := $ScrollContainer/Contents/Body
@onready var scoring := $ScrollContainer/Contents/Scoring
@onready var values := $ScrollContainer/Contents/Values

var pressing := false

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var full := ChesscadeModel.get_rules()
	var parts := full.split('\n\n')
	body.text = parts[0]
	body.text = body.text.replace('\n', ' ')
	scoring.text = parts[1]
	scoring.text = scoring.text.replace('+', '+\n')
	scoring.text = scoring.text.replace('x', ' x\n')
	values.text = parts[2]	

func _input(event: InputEvent) -> void:
	if not visible:
		return
	
	if event is InputEventKey and event.pressed :        
		if event.keycode in [KEY_DOWN, KEY_PAGEDOWN] :
			$ScrollContainer.scroll_vertical += 100
		elif event.keycode in [KEY_UP, KEY_PAGEUP]:	
			$ScrollContainer.scroll_vertical -= 100
		elif event.keycode == KEY_HOME:
			$ScrollContainer.scroll_vertical = 0
		elif event.keycode == KEY_END:	
			$ScrollContainer.set_v_scroll (999999)
	elif event is InputEventMouseMotion:
		if abs(event.relative.y) > 0 and Input.is_action_pressed('left_click'):
			$ScrollContainer.scroll_vertical -= event.relative.y
			pressing = abs(event.relative.y) > $ScrollContainer.scroll_deadzone

func _on_button_down() -> void:
	pressing = visible

func _on_button_up() -> void:
	if pressing:
		visible = false
