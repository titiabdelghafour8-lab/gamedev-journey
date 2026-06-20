shader_type canvas_item;


uniform vec3 second_color: source_color = vec3(0.5, 0.0, 0.0);
uniform vec3 minute_color: source_color = vec3(0.0, 0.5, 0.0);
uniform vec3 hour_color: source_color = vec3(0.0, 0.0, 0.5);

const float SMOOTHING = 1.5;
const float RING_THICKNESS = 0.1;
varying flat vec4 clock_data;


float smooth_above(float threshold, float value) {
	return smoothstep(threshold, threshold + fwidth(SMOOTHING * value), value);
}

float smooth_below(float treshold, float value)
{
	return smoothstep(treshold, treshold - fwidth(SMOOTHING * value), value);
}

float get_ring_weight(float treshold, float value, float ring_thickness)
{
	return smooth_below(treshold, value) * smooth_above(treshold - ring_thickness, value);
}

void vertex()
{
	clock_data = COLOR;
}

void fragment() {
	float radius = 2.0 * distance(UV, vec2(0.5));
	float choice = clock_data.a;
	COLOR = texture(TEXTURE,vec2(
		choice > 0.5 ? UV.x: 1.0 - UV.x,
		choice < 0.25 ? UV.y: 1.0 - UV.y
	) 
	);
	float mixer1 = get_ring_weight(clock_data.x, radius, RING_THICKNESS);
	float mixer2 = get_ring_weight(clock_data.y, radius, RING_THICKNESS);
	float mixer3 = get_ring_weight(clock_data.z, radius, RING_THICKNESS);
	COLOR.rgb = mix(COLOR.rgb, second_color, mixer1);
	COLOR.rgb = mix(COLOR.rgb, minute_color, mixer2);
	COLOR.rgb = mix(COLOR.rgb, hour_color, mixer3);
	COLOR.a = smooth_below(1.0, radius);
}