shader_type canvas_item;

uniform vec4 tint_color = vec4(1.0, 1.0, 1.0, 1.0);

void fragment() {
    vec4 tex_color = texture(TEXTURE, UV);
    
    if (tex_color.a > 0.0) {
        // Multiply RGB by tint, keep original alpha
        COLOR = vec4(tint_color.rgb, tex_color.a * tint_color.a);
    } else {
        // Keep fully transparent pixels untouched
        COLOR = tex_color;
    }
}
