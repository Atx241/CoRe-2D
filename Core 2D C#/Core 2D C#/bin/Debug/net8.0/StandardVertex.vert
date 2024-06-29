#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;
uniform bool scaleWithScreen;
uniform bool anchoredEntity;
uniform vec2 anchor;
uniform vec2 position;
uniform vec2 scale;
uniform float rotation;
uniform float aspectRatio;
uniform vec2 cameraPosition;
uniform float cameraZoom;
out vec2 texCoords;
void main() {
	float x = (aPos.x) * scale.x;
	float y = (aPos.y) * scale.y;
	gl_Position = vec4(((x * cos(-rotation / (180/3.141592)) - y * sin(-rotation / (180/3.141592))) + (position.x - (anchoredEntity ? 0 : cameraPosition.x))) / (scaleWithScreen ? 1 : aspectRatio) * 2,(y * cos(-rotation / (180/3.141592)) + x * sin(-rotation / (180/3.141592)) + (position.y - (anchoredEntity ? 0 : cameraPosition.y))) * 2,0,anchoredEntity ? 1 : cameraZoom) + vec4(anchor * 2, 0, 1);
	texCoords = aTex;
}