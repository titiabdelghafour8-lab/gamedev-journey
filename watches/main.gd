extends Node2D

@export var clock_scene: PackedScene
## the radius of the clock
@export var clock_radius := 128.0


@export_group("Clock Instances")
@export_range(0.1, 1.0) var scale_min := 0.25
@export_range(0.1, 1.0) var scale_max := 1.0
@export_range(-10.0, 10.0) var time_scale_min := -10.0
@export_range(-10.0, 10.0) var time_scale_max := 10.0

func _ready() -> void:
	get_window()	.size_changed.connect(_on_size_changed)
	_on_size_changed()

func _on_size_changed():
	var window_size := get_window().size
	($Bottom as Node2D).position.y = window_size.y + 2.0 * clock_radius
	var ground := $Ground as Node2D
	var ground_shape = $Ground/CollisionShape2D as Node2D
	ground.scale = Vector2.ONE
	ground_shape.shape.size = Vector2(window_size.x, clock_radius)
	ground.position = Vector2(
			0.5 * window_size.x,
			window_size.y + 0.5 * clock_radius
	)
	
	
func _on_buttom_body_entered(body: Node2D) -> void:
	body.queue_free()


func _on_spawn_timer_timeout() -> void:
	var window_size := get_window().size
	var clock := clock_scene.instantiate() as Clock
	clock.position = Vector2(
		randf_range(clock_radius, window_size.x - clock_radius), 
		-3.0 * clock_radius
		)
	clock.time_scale = randf_range(time_scale_min, time_scale_max)
	clock.set_uniform_scale(randf_range(scale_min, scale_max))
	add_child(clock)
	
	

	
