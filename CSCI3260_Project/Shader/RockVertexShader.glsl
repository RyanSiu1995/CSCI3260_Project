#version 440

in layout(location=0) vec3 position;
in layout(location=1) vec2 vertexUV;
in layout(location=2) vec3 normal;
in layout(location=3) mat4 instanceMatrix;

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


out vec2 UV;
out vec3 normalOut;
out vec3 vertex;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position  = projectionMatrix*viewMatrix*modelTransformMatrix*instanceMatrix * v;
	mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix*instanceMatrix)));
	normalOut = normalMatrix*normal;
	vertex = vec3(modelTransformMatrix*instanceMatrix*vec4(position,1.0));
	UV = vertexUV;
}