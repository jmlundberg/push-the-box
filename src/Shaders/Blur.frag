#extension GL_ARB_shading_language_420pack : enable

layout(binding = 16) uniform sampler2D textureData;

in vec2 textureCoords;
in vec2 blurTextureCoords[14];

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(0.0);
    fragmentColor += texture(textureData, blurTextureCoords[ 0])*0.0044299121055113265;
    fragmentColor += texture(textureData, blurTextureCoords[ 1])*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[ 2])*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[ 3])*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[ 4])*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[ 5])*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[ 6])*0.147308056121;
    fragmentColor += texture(textureData, textureCoords        )*0.159576912161;
    fragmentColor += texture(textureData, blurTextureCoords[ 7])*0.147308056121;
    fragmentColor += texture(textureData, blurTextureCoords[ 8])*0.115876621105;
    fragmentColor += texture(textureData, blurTextureCoords[ 9])*0.0776744219933;
    fragmentColor += texture(textureData, blurTextureCoords[10])*0.0443683338718;
    fragmentColor += texture(textureData, blurTextureCoords[11])*0.0215963866053;
    fragmentColor += texture(textureData, blurTextureCoords[12])*0.00895781211794;
    fragmentColor += texture(textureData, blurTextureCoords[13])*0.0044299121055113265;
}
