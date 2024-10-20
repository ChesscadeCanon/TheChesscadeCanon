@tool
extends CenterContainer

@export var title := 'Title': get = get_title, set = set_title

func get_title()->String:
	return $VBoxContainer/Title.text
	
func set_title(to: String)->void:
	$VBoxContainer/Title.text = to

func show_output(output: String)->void:
	$VBoxContainer/ReadoutCenter/ReadoutLabel.text = output

@export var readout_label_settings:LabelSettings: get = get_readout_label_settings, set = set_readout_label_settings

func get_readout_label_settings()->LabelSettings:
	return $VBoxContainer/ReadoutCenter/ReadoutLabel.label_settings
	
func set_readout_label_settings(to: LabelSettings)->void:
	if not $VBoxContainer/ReadoutCenter/ReadoutLabel:
		return
	$VBoxContainer/ReadoutCenter/ReadoutLabel.label_settings = to

func show_score()->void:
	var current_score :int= ChesscadeModel.get_score()
	$VBoxContainer/ReadoutCenter/ReadoutLabel.text = str(current_score)
	
func show_high_score(score: int)->void:
	$VBoxContainer/ReadoutCenter/ReadoutLabel.text = str(score)

func show_time()->void:
	var text := ''
	var current_time :float= ChesscadeModel.get_time()
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
	text = zero_hour + str(min(int(hours), 99)) + ':'
	text += zero_minute + str(min(int(minutes), 59)) + ':'
	text += zero_second + str(min(int(seconds), 59)) + ':'
	text += zero_centisecond + str(min(int(centiseconds), 99))
	$VBoxContainer/ReadoutCenter/ReadoutLabel.text = text
