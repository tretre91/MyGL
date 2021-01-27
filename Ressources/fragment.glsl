#version 330 core

in vec2 inputTexCoords;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float opacity;

void main() {
    FragColor = mix(texture(texture1, inputTexCoords), texture(texture2, inputTexCoords), opacity);
};