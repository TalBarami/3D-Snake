#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;
varying vec4 position0;

//out vec4 FragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform mat4 Normal;
uniform vec4 eyePosition;

uniform sampler2D sampler;

void main()
{
	vec3 texColor = texture2D(sampler, texCoord0).rgb;

    vec3 L = normalize(lightDirection);
	vec3 diffuse = lightColor * max(dot(-L, normal0), 0.0f);
	vec4 intersection = Normal * vec4(position0.xyz, 1.0f);
	vec3 v = normalize(eyePosition - intersection).xyz;
	vec3 r = reflect(L, normalize(normal0));
	float specular = max(dot(v,r), 0.0f);
	vec3 result = diffuse + pow(specular, 2.0f) * vec3(0.25f,0.25f,0.25f);

	vec3 color = texColor + result;
	gl_FragColor = clamp(vec4(color, 1.0f), 0.0f, 1.0f);

}