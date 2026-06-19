shader_type canvas_item;

const float SMOOTHING = 1.5;
const float RING_THICKNESS = 0.1;

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

void fragment() {
	float radius = 2.0 * distance(UV, vec2(0.5));
	COLOR = vec4(1.0);
	float mixer = get_ring_weight(0.2, radius, RING_THICKNESS);
	COLOR.rgb = mix(COLOR.rgb, vec3(0.0, 0.5, 0.0), mixer);
	COLOR.a = smooth_below(1.0, radius);
}