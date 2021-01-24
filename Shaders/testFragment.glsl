#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main(){
	color = vec4(textColor, texture(tex, texCoords).r);
}