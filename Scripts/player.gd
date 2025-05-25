class_name PlayerController extends CharacterBody2D
# Nodes 
@onready var ANIMATOR:AnimatedSprite2D = $AnimatedSprite2D

var abilities:Array[Ability]

@export var shadowMaterial:ShaderMaterial

func _ready() -> void:
	populate()

func _process(delta: float) -> void:
	if not is_on_floor():
		if velocity.y > 0:
			ANIMATOR.play("Fall")
		else:
			ANIMATOR.play("Jump")
	
	for a in abilities:
		if !a.is_physics && a.enabled:
			a.update(delta)

func _physics_process(delta: float) -> void:
	for a in abilities:
		if a.is_physics && a.enabled:
			a.update(delta)
	move_and_slide()

func populate():
	abilities = []
	for child in get_children():
		if child is Ability:
			abilities.append(child)
	
	for a in abilities:
		if a.enabled:
			a.start()

func deactivate(ab:String):
	for a in abilities:
		if a.name == ab:
			a.enabled = false

func activate(ab:String):
	for a in abilities:
		if a.name == ab:
			a.enabled = true

func get_ability(ab:String):
	for a in abilities:
		if a.name == ab:
			return a

func create_shadow(duration:float):
	var shadow:AnimatedSprite2D = ANIMATOR.duplicate()
	shadow.stop()
	shadow.autoplay = ""
	shadow.position = ANIMATOR.global_position
	shadow.z_index -= 1
	var t:Timer = Timer.new()
	t.one_shot = true
	t.wait_time = duration
	t.connect("timeout", func(): shadow.queue_free())
	shadow.add_child(t)
	shadow.material = shadowMaterial
	add_sibling(shadow)
	t.start()
