void fullScreenTriangle(out vec4 position, out vec2 textureCoords) {
    /* Stolen from https://twitter.com/g_truc/status/194469385191174145 */
    vec2 xy = mix(vec2(-1.0), vec2(3.0), bvec2(gl_VertexID == 1, gl_VertexID == 2));
    position = vec4(xy, 0.0, 1.0);
    textureCoords = xy*0.5 + vec2(0.5);
}
