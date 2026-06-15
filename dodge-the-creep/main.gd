extends Node

@export var mob_scene : PackedScene 
var score = 0

func _ready() -> void:
	$HUD.update_score(score)
	$HUD.show_message("Get Ready")
# Called every frame. 'delta' is the elapsed time since the previous frame.

func _process(delta: float) -> void:
	pass


func game_over() -> void:
	$scoreTimer.stop()
	$MobTimer.stop()
	$HUD.show_game_over()
	$Music.stop()
	$DeathSound.play()
	

func new_game():
	score = 0
	$HUD.update_score(score)
	$Player.start($StartPosition.position)
	$startTimer.start()
	get_tree().call_group("mobs", "queue_free")
	$Music.play()
	


func _on_mob_timer_timeout() -> void:
	#instanciating the mob and in a random location accros the MobSpawnLocation 
	var mob = mob_scene.instantiate()
	var mob_spawn_location = $MobPath/MobSpawnLocation
	
	#setting the position and progress 
	mob_spawn_location.progress = randf()
	mob.position = mob_spawn_location.position
	
	#setting up random direction for the mobs to spawn perpendicularily
	var direction = mob_spawn_location.rotation + PI / 2
	direction += randf_range(-PI / 4, PI / 4) # adding some randomness to the direction 
	mob.rotation = direction
	#setting a random velocity from the suggested values and rotatin the velocity vector with the random direction set
	var velocity = Vector2(randf_range(150.0, 250.0), 0.0)
	mob.linear_velocity = velocity.rotated(direction)
	#adding the mob instance after setting up everything to the main scene
	add_child(mob)
	
	
	
	
func _on_score_timer_timeout() -> void:
	score += 1
	$HUD.update_score(score)
	

func _on_start_timer_timeout() -> void:
	$MobTimer.start()
	$scoreTimer.start()
