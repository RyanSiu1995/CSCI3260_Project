#version 440

in vec2 UV;
in vec3 normalOut;
in vec3 vertex;

uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;
uniform sampler2D textureSampler3;
uniform vec3 lightPositionWorld;
uniform vec3 lightPositionWorld2;
uniform vec3 lightPositionWorld3;
uniform vec3 eyePositionWorld;
uniform int selfIlluminated;
uniform int normalMapping_flag;
uniform int multipleTexture;

out vec4 color;

void main()
{
	vec3 normal = normalize(normalOut);
	if(normalMapping_flag == 1){
		normal = texture(textureSampler2, UV).rgb;
		normal = normalize(2.0*normal-1.0);
	}
	vec3 textureColor ;
	if(multipleTexture == 1){
		textureColor = (0.2*texture( textureSampler1, UV ) + 0.5*texture( textureSampler2, UV ) + 0.3*texture( textureSampler3, UV )).rgb;
	}
	else{
		textureColor = texture( textureSampler1, UV ).rgb;
	}

	vec4 colortemp = vec4(textureColor,1.0);


	vec3 ambient;
	if(selfIlluminated == 0)
		ambient = textureColor * 0.7;
	else
		ambient = textureColor * 0.01;

	vec3 lightVectorWorld = normalize(lightPositionWorld - vertex);
	float brightness = max(dot(lightVectorWorld, normal), 0.0);
	brightness = brightness*1;
	vec3 diffuse = vec3(brightness,brightness,brightness);
	diffuse = diffuse*textureColor;

	vec3 view = normalize(eyePositionWorld - vertex);
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normal);
	vec3 halfway = normalize(reflectedLightVectorWorld+view);
	float s = pow(max(dot(normal, halfway),0.0),32.0);
	vec3 specular = vec3(0.2)*s;

	vec3 lightVectorWorld2 = normalize(lightPositionWorld2 - vertex);
	float brightness2 = max(dot(lightVectorWorld2, normal), 0.0);
	brightness2 = brightness2 * 0.7;
	vec3 diffuse2 = vec3(0.87 * brightness2, 0.1 * brightness2 ,0);
	diffuse2 = diffuse2*textureColor;

	vec3 view2 = normalize(eyePositionWorld - vertex);
	vec3 reflectedLightVectorWorld2 = reflect(-lightVectorWorld2, normal);
	vec3 halfway2 = normalize(reflectedLightVectorWorld2 + view2);
	float s2 = pow(max(dot(normal, halfway2),0.0),32.0);
	vec3 specular2 = vec3(0.2)*s2;

	vec3 lightVectorWorld3 = normalize(lightPositionWorld3 - vertex);
	float brightness3 = max(dot(lightVectorWorld3, normal), 0.0);
	brightness3 = brightness3 * 1;
	vec3 diffuse3 = vec3(0, 0.1 * brightness3 ,0.5 * brightness3);
	diffuse3 = diffuse3*textureColor;

	vec3 view3 = normalize(eyePositionWorld - vertex);
	vec3 reflectedLightVectorWorld3 = reflect(-lightVectorWorld3, normal);
	vec3 halfway3 = normalize(reflectedLightVectorWorld3 + view3);
	float s3 = pow(max(dot(normal, halfway3),0.0),32.0);
	vec3 specular3 = vec3(0.2)*s3;

	color = vec4(ambient + diffuse + specular + diffuse2 + specular2 + diffuse3 + specular3, 1.0f);

}
