#version 330

in vec4 color;
in vec4 position;

out vec4 fColor;

void main()
{
	vec4 c = color;
	float d = 0.05f;

	if((position.x>0.5-d || position.x<-0.5+d) && (position.y>0.5-d || position.y<-0.5+d))
		c = vec4(0,1,0,1);
	if((position.x>0.5-d || position.x<-0.5+d) && (position.z>0.5-d || position.z<-0.5+d))
		c = vec4(0,1,0,1);
	if((position.z>0.5-d || position.z<-0.5+d) && (position.y>0.5-d || position.y<-0.5+d))
		c = vec4(0,1,0,1);
	

	fColor = c;
}