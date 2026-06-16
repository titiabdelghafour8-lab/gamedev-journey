class_name Clock

extends RigidBody2D

enum StartTimerState 
{
	SYSTEM_TIME, 
	RANDOM_TIME,
	FIXED_TIME, 
	OFFSET_TIME,
}


@export var time_state := StartTimerState.SYSTEM_TIME

@export var time_scale := 1.0

@export_group("Setting up the start timer for the offset and system time")
@export_range(0, 11) var start_hour: int= 0
@export_range(0, 59) var start_minute: int= 0
@export_range(0, 59) var start_second: int= 0


@export_group("Nodes")
@export var collision_shape : CollisionShape2D
@export var visualization : Node2D
@export_subgroup("Arms")
@export var seconds_arm : Node2D
@export var minutes_arm : Node2D
@export var hours_arm : Node2D


var seconds := 0.0 
var start_time := Dictionary()


func set_uniform_scale(scale_factor: float) -> void:
	var scale_vector = Vector2(scale_factor, scale_factor)
	collision_shape.scale = scale_vector
	visualization.scale = scale_vector
	mass = scale_factor * scale_factor

func _ready():
	if time_state == StartTimerState.RANDOM_TIME:
		seconds = randf_range(0, 43200)
	else:
		if time_state == StartTimerState.SYSTEM_TIME:
			var current_time = Time.get_time_dict_from_system()
			seconds = current_time.hour * 3600 + current_time.minute * 60 + current_time.second
		if time_state != StartTimerState.FIXED_TIME:
			seconds += start_hour * 3600 + start_minute * 60 + start_second	
	
	

	
func _process(delta: float):
	seconds += delta * time_scale
	seconds_arm.rotation = fmod(seconds, 60.0) * TAU / 60.0
	minutes_arm.rotation = fmod(seconds / 60.0, 60.0) * TAU / 60.0
	hours_arm.rotation = fmod(seconds / 3600.0, 12.0)	* TAU / 12.0
	
