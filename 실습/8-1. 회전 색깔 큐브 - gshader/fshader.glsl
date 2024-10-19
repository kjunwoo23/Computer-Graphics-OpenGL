#version 330

in vec4 gColor;
in vec4 gPosition;

out vec4 fColor;

void main()
{
	vec4 c = gColor;
	float d = 0.05f;

	/*
	if((gPosition.x>0.5-d || gPosition.x<-0.5+d) && (gPosition.y>0.5-d || gPosition.y<-0.5+d))
		c = vec4(0,1,0,1);
	if((gPosition.x>0.5-d || gPosition.x<-0.5+d) && (gPosition.z>0.5-d || gPosition.z<-0.5+d))
		c = vec4(0,1,0,1);
	if((gPosition.z>0.5-d || gPosition.z<-0.5+d) && (gPosition.y>0.5-d || gPosition.y<-0.5+d))
		c = vec4(0,1,0,1);*/
	
	fColor =  c;
}