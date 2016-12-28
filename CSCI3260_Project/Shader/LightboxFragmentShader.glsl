#version 440

in vec2 UV;
in vec3 normalOut;
in vec3 vertex;

uniform sampler2D textureSampler1;
uniform int colorDecision;

out vec4 color;

void main()
{
	vec3 texture = texture(textureSampler1, UV).rgb;
	if(colorDecision == 1)
		color = vec4(texture, 1.0);
	else
	if(colorDecision == 2)
		color = vec4(0.87, 0.1, 0.0, 1.0);
	else
	if(colorDecision == 3)
		color = vec4(0.0, 0.1, 0.5, 1.0);
}
