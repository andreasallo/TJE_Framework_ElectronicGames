varying vec2 v_uv;
uniform float u_intensity; 

void main()
{
    
    float dist = length(v_uv - 0.5);

    // Funció smoothstep per suavitzar la vora.
    float vignette = smoothstep(0.25,0.65, dist); //comença a radi 0.4 fins a 0.8
    vignette *= u_intensity;
    gl_FragColor = vec4(0.0, 0.0, 0.0, vignette);
}