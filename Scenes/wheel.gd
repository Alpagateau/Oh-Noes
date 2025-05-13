extends Node2D

@export var fxs:Array[Effect]
@export var colors:Array[Color]
@export var radius:float = 30
@export var font_settings:LabelSettings

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
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
		new_quarter.position = position
		
		
		var txt:Label = Label.new()
		txt.label_settings = font_settings
		txt.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER 
		txt.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
		txt.size.x = radius
		txt.size.y = radius * tan(dtheta)
		txt.position.y = -txt.size.y / 2
		#txt.position.x = txt.size.x / 4
		txt.text = fxs[i].name
		add_child(new_quarter)
		new_quarter.add_child(txt)
		new_quarter.rotation = i * dtheta
		
		
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
