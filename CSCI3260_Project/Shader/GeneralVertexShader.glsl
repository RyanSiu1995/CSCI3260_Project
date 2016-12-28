#version 440

in layout(location=0) vec3 position;
in layout(location=1) vec2 vertexUV;
in layout(location=2) vec3 normal;

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 UV;
out vec3 normalOut;
out vec3 vertex;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position  = projectionMatrix*viewMatrix*modelTransformMatrix * v;
	mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix)));
	normalOut = normalMatrix*normal;
	vertex = vec3(modelTransformMatrix*vec4(position,1.0));
	UV = vertexUV;
}