extends Effect

@export var AbilityName:String
@export var EnableAbility:bool

var default = null

func start() -> void:
	super.start()
	player.get_ability(AbilityName).enabled = EnableAbility
