#ifndef NEW_GLSL
#define out varying
#endif

#ifndef RUNTIME_CONST
#define const
#endif

#ifdef NEW_GLSL
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 16) uniform sampler2D textureData;
#else
uniform sampler2D textureData;
#endif
#else
uniform vec2 imageSizeInverted;
#endif

out mediump vec2 textureCoords;
out mediump vec4 blurTextureCoords[7];

mediump vec2 coordinate(mediump float value) {
    #ifdef DIRECTION_HORIZONTAL
    return vec2(value, 0.0);
    #else
    return vec2(0.0, value);
    #endif
}

void main() {
    fullScreenTriangle();

    textureCoords = gl_Position.xy*0.5 + vec2(0.5);

    #ifdef DIRECTION_HORIZONTAL
    #ifdef NEW_GLSL
    const float increment = 1.0/float(textureSize(textureData, 0).x);
    #else
    const mediump float increment = imageSizeInverted.x;
    #endif
    #else
    #ifdef NEW_GLSL
    const float increment = 1.0/float(textureSize(textureData, 0).y);
    #else
    const mediump float increment = imageSizeInverted.y;
    #endif
    #endif

    for(int i = 0; i != 7; ++i) {
        blurTextureCoords[i].xy = textureCoords - coordinate(increment*float(i+1));
        blurTextureCoords[i].zw = textureCoords + coordinate(increment*float(i+1));
    }
}
