#version 410 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = model * vec4(vPosition, 1.0f);
	
	TexCoord = vec2( texCoord.x, 1.0 - texCoord.y );
}