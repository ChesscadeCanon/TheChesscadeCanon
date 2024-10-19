extends HBoxContainer

@onready var time :Label= $TimeCenter/TimeVBox/ReadoutCenter/TimeLabel
@onready var score :Label= $ScoreCenter/ScoreVBox/ReadoutCenter/ScoreLabel

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var current_score := ChesscadeModel.get_score()
	score.text = str(current_score)
	
	var current_time := ChesscadeModel.get_time()
	var hours :float= floor(current_time / 3600.0)
	var zero_hour := '' if hours >= 10.0 else '0'
	current_time -= hours * 3600.0
	var minutes :float= floor(current_time / 60.0)
	var zero_minute := '' if minutes >= 10.0 else '0'
	current_time -= minutes * 60.0
	var seconds :float= floor(current_time)
	var zero_second = '' if seconds >= 10.0 else '0'
	current_time -= seconds
	var centiseconds :float= floor(current_time * 100.0)
	var zero_centisecond := '' if centiseconds >= 10.0 else '0'
	time.text = zero_hour + str(min(int(hours), 99)) + ':'
	time.text += zero_minute + str(min(int(minutes), 59)) + ':'
	time.text += zero_second + str(min(int(seconds), 59)) + ':'
	time.text += zero_centisecond + str(min(int(centiseconds), 99))
