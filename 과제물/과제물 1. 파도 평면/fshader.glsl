#version 330

uniform float uTime;
uniform float uPlainSize;
uniform bool uWave;

in vec4 color;
in vec4 position;
in float gapY;

out vec4 fColor;

void main()
{
	vec4 c = color;

	if (uWave)
		if (gapY <= 0)
			c = vec4((c.r)*(1+gapY*4.5f), (c.g - 0.5f)*(1+gapY*4.5f)+0.5f, (c.b - 1)*(1+gapY*4.5f) + 1, 1);
		else
			c = vec4((c.r-1)*(1-gapY*4.5f)+1, (c.g - 0.5f)*(1-gapY*4.5f) + 0.5f, (c.b)*(1-gapY*4.5f), 1);
		
	c.a = 1;

	fColor = c;
}