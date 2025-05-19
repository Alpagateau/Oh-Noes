extends Ability

@export var speed:float = 0.001
@export var look_ahead:float = 0.1
@export var max_dist:float = 32

var base_offset:Vector2

@export var cam:Camera2D

func start() -> void:
	super.start()
	base_offset = cam.offset
	
func update(delta:float) -> void:
	#print(cam.offset, player.velocity)
	var lp:Vector2 = player.velocity * look_ahead
	cam.offset = cam.offset.lerp(lp + base_offset, delta * speed)
	
	if cam.offset.length() >= max_dist:
		#print("Too Long : ",cam.offset.length() )
		cam.offset = cam.offset.normalized() * max_dist
		
	
	pass
