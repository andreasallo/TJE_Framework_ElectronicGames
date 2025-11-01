
varying vec3 v_world_position;

uniform samplerCube u_texture;
uniform vec3 u_camera_position;

void main(){

	Vec3 dir=normalize( u_camera_position-v_world_position);
	Gl_FragColor=textureCube(u_texture,uv)
}