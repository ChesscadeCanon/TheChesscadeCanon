extends ChesscadeAdapter


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	reset()

func _exit_tree() -> void:
	clear()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
