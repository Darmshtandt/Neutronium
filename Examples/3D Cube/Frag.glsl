#version 400

uniform mat4 Projection;
uniform mat4 World;
uniform mat4 View;

in vec3 VertexTexCoords;

out vec4 Color;

uniform sampler2D ourTexture;

void main() {
	Color = texture(ourTexture, vec2(VertexTexCoords));
}