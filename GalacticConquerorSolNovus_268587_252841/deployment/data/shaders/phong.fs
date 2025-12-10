

void main () 
{
	vec4 final_color = applyLight();

	float dist = length(u_camera_pos - v_world_pos);
	float fog_factor = clamp(1.0 - (fogMaxDist - dist) / (fogMaxDist - fogMinDist, 0.0,	1.0);
	fog_value += u_fog_factor;

	fog_color = vec3();

	final_color = mix( final_color, fog_color, fog_factor );
}