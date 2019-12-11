#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D smileTexture;

void main()
{
    vec2 transformed = vec2(-TexCoord.x, TexCoord.y);
    FragColor = texture(smileTexture, transformed);
}
