#version 410 core
out vec4 FragColor;

in vec2 screen_uv;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, screen_uv * 0.5 + 0.5).rgb;
    FragColor = vec4(col, 1.0);
} 