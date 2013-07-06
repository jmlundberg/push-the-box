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
in mediump vec4 blurTextureCoords[7];

#ifdef NEW_GLSL
out vec4 fragmentColor;
#endif

void main() {
    fragmentColor  = vec4(0.0);
    fragmentColor += texture(textureData, blurTextureCoords[6].xy)*0.0044299121055113265;
    fragmentColor += texture(textureData, blurTextureCoords[5].xy)*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[4].xy)*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[3].xy)*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[2].xy)*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[1].xy)*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[0].xy)*0.147308056121;
    fragmentColor += texture(textureData, textureCoords          )*0.159576912161;
    fragmentColor += texture(textureData, blurTextureCoords[0].zw)*0.147308056121;
    fragmentColor += texture(textureData, blurTextureCoords[1].zw)*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[2].zw)*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[3].zw)*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[4].zw)*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[5].zw)*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[6].zw)*0.0044299121055113265;
}
