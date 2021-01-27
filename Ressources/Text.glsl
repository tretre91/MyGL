// Vertex shader
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCoords = aTex;
};

// Fragment shadere
#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main() {
    color = vec4(textColor, texture(tex, texCoords).r);
};