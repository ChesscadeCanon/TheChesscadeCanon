extends Node2D

@export var stretch :float= 1.0: set=set_stretch
@export var snap :float= 1.0: set=set_snap

func set_stretch(to: float)->void:
	stretch = to
	stretch_snap()
	
func set_snap(to: float)->void:
	snap = to
	stretch_snap()

func stretch_snap()->void:
	for child in get_children():
		child.scale = Vector2.ONE * stretch
	scale = Vector2.ONE / stretch * snap

func _ready()->void:
	stretch_snap()
