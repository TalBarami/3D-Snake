#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 color;
attribute vec3 weight;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;

uniform mat4 MVP;
uniform mat4 Normal;

uniform int links_num;
uniform int link;
uniform mat4[32] T;

void main()
{
	
	if(link < 0 || link >= links_num - 1) {
		gl_Position = MVP * vec4(position, 1.0);
	}
	else{
		mat4 result = weight[1] * T[link];
		if(link > 0){
			result += weight[0] * T[link - 1];
		}
		if(link < links_num - 1){
			result += weight[2] * T[link + 1];
		}
		
		gl_Position = result * vec4(position, 1.0);
	}
	
	texCoord0 = texCoord;
	color0 = color;
	weight0 = weight;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}
