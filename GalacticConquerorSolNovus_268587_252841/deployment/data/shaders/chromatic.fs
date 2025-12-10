varying vec2 v_uv;
uniform sampler2D u_texture;
uniform float u_amount; // Intensitat de l'efecte 

void main()
{
	vec2 dist = v_uv - 0.5;
	vec2 offset = dist * u_amount * 0.10; 

	float r = texture2D(u_texture, v_uv + offset).r;
	float g = texture2D(u_texture, v_uv).g;
	float b = texture2D(u_texture, v_uv - offset).b;

	gl_FragColor = vec4(r, g, b, 1.0);
}