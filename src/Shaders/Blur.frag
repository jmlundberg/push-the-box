#ifndef NEW_GLSL
#define in varying
#define fragmentColor gl_FragColor
#define texture texture2D
#endif

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 16) uniform sampler2D textureData;
#else
uniform sampler2D textureData;
#endif

in mediump vec2 textureCoords;
in mediump vec2 blurTextureCoords[14];

#ifdef NEW_GLSL
out vec4 fragmentColor;
#endif

void main() {
    fragmentColor = vec4(0.0);
    fragmentColor += texture(textureData, blurTextureCoords[ 0])*0.0044299121055113265;
    fragmentColor += texture(textureData, blurTextureCoords[ 1])*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[ 2])*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[ 3])*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[ 4])*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[ 5])*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[ 6])*0.147308056121;
    fragmentColor += texture(textureData, gl_FragCoord.xy      )*0.159576912161;
    fragmentColor += texture(textureData, blurTextureCoords[ 7])*0.147308056121;
    fragmentColor += texture(textureData, blurTextureCoords[ 8])*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[ 9])*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[10])*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[11])*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[12])*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[13])*0.0044299121055113265;
}
