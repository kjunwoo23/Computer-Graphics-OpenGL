#version 330

uniform float uTime;
uniform bool uWave;

in vec4 vPosition;
in vec4 vColor;

out vec4 color;
out vec4 position;
out float gapY;

void main()
{

	float uTheta1 = -uTime*35;

	float rad1 = uTheta1 / 180.0 * 3.141592;
	mat4 m = mat4(1.0);							// y - rotation
	m[0][0] = cos(rad1);m[1][0] = 0;		m[2][0] = sin(rad1);m[3][0] = 0;
	m[0][1] = 0;		m[1][1] = 1;		m[2][1] = 0;		m[3][1] = 0;
	m[0][2] =-sin(rad1);m[1][2] = 0;		m[2][2] = cos(rad1);m[3][2] = 0;
	m[0][3] = 0;		m[1][3] = 0;		m[2][3] = 0;		m[3][3] = 1;

	float rad2 = -45 / 180.0 * 3.141592;
	mat4 n = mat4(1.0);							// x - rotation
	n[0][0] = 1;		n[1][0] = 0;		n[2][0] = 0;		n[3][0] = 0;
	n[0][1] = 0;		n[1][1] = cos(rad2);n[2][1] =-sin(rad2);n[3][1] = 0;
	n[0][2] = 0;		n[1][2] = sin(rad2);n[2][2] = cos(rad2);n[3][2] = 0;
	n[0][3] = 0;		n[1][3] = 0;		n[2][3] = 0;		n[3][3] = 1;
	

	gl_Position = n * m * vPosition;
	if (uWave)
		if (sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2)) < 0.75f)
			gl_Position.y += 0.3 * sin(7.5f * (uTime + 1) - 20 * sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2))) * (0.75f - sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2)));
	
	if (sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2)) < 0.75f)
		gapY = 0.3 * sin(7.5f * (uTime + 1) - 20 * sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2))) * (0.75f - sqrt(pow(vPosition.x, 2) + pow(vPosition.z, 2)));
	else
		gapY = 0;

	color = vColor;
	position = vPosition;
}