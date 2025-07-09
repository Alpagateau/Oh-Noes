class_name PlayerController extends CharacterBody2D
# Nodes 
@onready var ANIMATOR:AnimatedSprite2D = $AnimatedSprite2D

var abilities:Array[Ability]
var enables:Array[bool]

@export var shadowMaterial:ShaderMaterial
@export var shadowPrefab:PackedScene

var start_pos:Vector2
var start_vel:Vector2

func _ready() -> void:
	enables = []
	populate()
	start_pos = position
	start_vel = velocity

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
	var isempt = (enables == [])
	abilities = []
	for child in get_children():
		if child is Ability:
			abilities.append(child)
	
	for a in len(abilities):
		if isempt:
			enables.append(abilities[a].enabled)
		else:
			abilities[a].enabled = enables[a]
		if abilities[a].enabled:
			abilities[a].start()

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
	var shadow = shadowPrefab.instantiate()
	var part:CPUParticles2D = shadow.get_child(0)
	part.lifetime = duration
	part.emitting = true
	shadow.position = ANIMATOR.global_position
	shadow.z_index -= 1
	var t:Timer = Timer.new()
	t.one_shot = true
	t.wait_time = duration
	t.connect("timeout", func(): shadow.queue_free())
	shadow.add_child(t)
	add_sibling(shadow)
	t.start()

func reload():
	position = start_pos
	velocity = start_vel
	get_ability("CameraController").reset()
	print(position)
	populate()
