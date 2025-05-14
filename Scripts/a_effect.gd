extends Effect

@export var abi:String

func start() -> void:
	super.start()
	world.player.get_ability(abi).queue_free()
