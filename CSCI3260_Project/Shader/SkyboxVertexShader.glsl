#version 440

in layout(location=0) vec3 position;

uniform mat4 modelTransformMatrix;

out vec3 textureCor;

void main()
{
	vec4 position_v4 = modelTransformMatrix * vec4(position, 1.0);
	gl_Position = position_v4;
	textureCor = position;
}