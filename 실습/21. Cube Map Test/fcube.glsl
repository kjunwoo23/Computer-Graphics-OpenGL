#version 330

in  vec4 color;
in  vec3 pos;
out vec4 fColor;

uniform samplerCube uCube;

void
main()
{
	vec3 dir = pos;
	dir.y = -dir.y;

	fColor = texture(uCube, dir);
}
