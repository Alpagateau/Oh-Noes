@tool
extends Node2D

@export var fxs:Array[Effect]
@export var colors:Array[Color]
@export var radius:float = 30
@export var font_settings:LabelSettings

@export var min_speed:float = 1 
@export var max_speed:float = 3
@export var deccel:float = 2

@export_category("Debug")
@export_tool_button("Generate Wheel") var gen = self.generate
@export_tool_button("Rotate Wheel") var rot = self.launch

var is_turnin:bool = false
var speed:float = 0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	generate()
	launch()

func generate():
	print("Generating")
	var children = get_children()
	for child in children:
		child.free()
	var l:int = len(fxs)
	var dtheta:float = 2*PI / l
	
	for i in range(l):
		var new_quarter:Polygon2D = Polygon2D.new()
		var vertex:Array[Vector2] = [Vector2(0,0)]
		for v in range(16):
			vertex.append(Vector2(
				radius *  cos( (dtheta*(v - 8))/15 ), 
				radius *  sin( (dtheta*(v - 8))/15 )
				))
		vertex.append(Vector2(0, 0))
		new_quarter.polygon = vertex
		new_quarter.color = colors[i%len(colors)]
		#new_quarter.position = position
		
		
		var txt:Label = Label.new()
		txt.label_settings = font_settings
		txt.horizontal_alignment = HORIZONTAL_ALIGNMENT_RIGHT 
		txt.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
		txt.size.x = radius * 2
		txt.size.y = radius * tan(dtheta) * 2
		txt.scale *= 0.5
		txt.position.y = -txt.size.y / 4
		#txt.position.x = txt.size.x / 4
		if fxs[i] != null:
			txt.text = fxs[i].name
		else:
			txt.text = "DEBUG"
		add_child(new_quarter)
		new_quarter.add_child(txt)
		new_quarter.rotation = i * dtheta
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if is_turnin:
		rotate(speed * delta)
		speed -= deccel * delta
		if speed <= 0:
			speed = 0 
			is_turnin = false
	pass

func launch():
	speed = randf_range(min_speed, max_speed)
	is_turnin = true
	pass
