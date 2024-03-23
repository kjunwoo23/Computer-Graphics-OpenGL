#version 330

in vec3 N3; 
in vec3 T3;
in vec3 L3; 
in vec3 V3; 
in vec2 texCoord;
out vec4 fColor;

uniform mat4 uModelMat; 
uniform mat4 uProjMat; 
uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform float uShininess; 
uniform sampler2D uTexture;
uniform sampler2D uTexBump;

void main()
{
	vec3 N = normalize(N3); 
/*	
	vec3 T = normalize(T3);
	vec3 B = cross(N, T);		

	vec4 bump = texture(uTexBump, texCoord);
	vec4 bump_u = texture(uTexBump, texCoord + vec2(0.01,0));
	vec4 bump_v = texture(uTexBump, texCoord + vec2(0,0.01));
	float du = bump_u.r - bump.r;
	float dv = bump_v.r - bump.r;
	float bump_scale = 1.f;
	N = normalize(N + (du*T + dv*B)*bump_scale);
*/
	vec3 L = normalize(L3);
	vec3 V = normalize(V3); 
	vec3 H = normalize(V+L); 
	
    float NL = max(dot(N, L), 0); 
	float VR = pow(max(dot(H, N), 0), uShininess); 

	fColor = uAmb + uDif*NL + uSpc*VR; 
	fColor.w = 1; 
	fColor *= texture(uTexture, texCoord);
}
