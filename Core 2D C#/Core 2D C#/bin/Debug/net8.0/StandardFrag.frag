#version 330 core
out vec4 FragCol;
in vec2 texCoords;
uniform sampler2D text;
void main() {
	FragCol = texture(text,texCoords);
}