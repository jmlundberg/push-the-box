#extension GL_ARB_shading_language_420pack : enable

layout(binding = 16) uniform sampler2D textureData;

out vec2 textureCoords;
out vec2 blurTextureCoords[14];

vec2 coordinate(float value) {
    #ifdef DIRECTION_HORIZONTAL
    return vec2(value, 0.0);
    #else
    return vec2(0.0, value);
    #endif
}

void main() {
    fullScreenTriangle(gl_Position, textureCoords);

    #ifdef DIRECTION_HORIZONTAL
    const float increment = 1.0/float(textureSize(textureData, 0).x);
    #else
    const float increment = 1.0/float(textureSize(textureData, 0).y);
    #endif
    for(int i = 0; i != 7; ++i)
        blurTextureCoords[6-i] = textureCoords + coordinate(increment*(i+1));

    for(int i = 0; i != 7; ++i)
        blurTextureCoords[7+i] = textureCoords + coordinate(increment*(i+1));
}
