#version 330 core

out vec4 finalColor;

in vec2 fragmentTexture;

uniform sampler2D gameTexture;

void main(){
	finalColor = texture(gameTexture, fragmentTexture);
}
