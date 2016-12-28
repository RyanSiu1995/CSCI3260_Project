#version 440

in vec3 textureCor;

uniform samplerCube skybox;

out vec4 color;

void main()
{
	color = texture(skybox, textureCor);
}