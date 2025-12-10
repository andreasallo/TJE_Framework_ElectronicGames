
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform float u_time;
uniform float u_mask;

void main()
{
	vec2 uv = v_uv;

	vec4 final_color = vec4(0.0);
	float mask = u_mask;

	if (v_uv.y > mask) { // u_mask!!!!!!!!!! // u_mask amb la llògica implementada
		final_color = u_color; // si uv és més gran que el threshold mask, pinta el color de la barra buida
	}
	else {
		final_color = mix(vec4(1.0,1.0,0.0,1.0), vec4(1.0,1.0,0.0,1.0), mask); //u_mask!!!!!!!!!!! // si uv és més petita, pinta aquella part del color de la barra + mix per què baixi
	}

	//gl_FragColor = texture2D( u_texture, uv );
	gl_FragColor = final_color;
}
