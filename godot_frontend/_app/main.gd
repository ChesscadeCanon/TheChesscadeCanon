extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	ChesscadeModel.begin_game()
	ChesscadeModel.process_mode = Node.PROCESS_MODE_INHERIT
