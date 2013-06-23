#include "Blur.h"
#include <Utility/Assert.h>
#include <Utility/Resource.h>
#include <Shader.h>

namespace PushTheBox { namespace Shaders {

Blur::Blur(Direction direction) {
    Utility::Resource rs("PushTheBoxShaders");

    Shader vert(Version::GL330, Shader::Type::Vertex);
    vert.addSource(direction == Direction::Horizontal ? "#define DIRECTION_HORIZONTAL\n" : "")
        .addSource(rs.get("FullScreenTriangle.glsl"))
        .addSource(rs.get("Blur.vert"));
    CORRADE_INTERNAL_ASSERT_OUTPUT(vert.compile());
    attachShader(vert);

    Shader frag(Version::GL330, Shader::Type::Fragment);
    frag.addSource(rs.get("Blur.frag"));
    CORRADE_INTERNAL_ASSERT_OUTPUT(frag.compile());
    attachShader(frag);

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());
}

}}
