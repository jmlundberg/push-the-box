#include "Blur.h"

#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Context.h>
#include <Magnum/Extensions.h>
#include <Magnum/Shader.h>
#include <Magnum/Texture.h>
#include <Magnum/Math/Vector2.h>

namespace PushTheBox { namespace Shaders {

namespace {
    enum: Int {
        TextureLayer = 16,
    };
}

Blur::Blur(Direction direction) {
    Utility::Resource mr("MagnumShaders");
    Utility::Resource rs("PushTheBoxShaders");

    #ifndef MAGNUM_TARGET_GLES
    const Version v = Context::current()->supportedVersion({Version::GL320, Version::GL210});
    #else
    const Version v = Context::current()->supportedVersion({Version::GLES300, Version::GLES200});
    #endif

    Shader vert(v, Shader::Type::Vertex);
    Shader frag(v, Shader::Type::Fragment);

    vert.addSource(direction == Direction::Horizontal ? "#define DIRECTION_HORIZONTAL\n" : "")
        .addSource(mr.get("compatibility.glsl"))
        .addSource(mr.get("FullScreenTriangle.glsl"))
        .addSource(rs.get("Blur.vert"));
    frag.addSource(mr.get("compatibility.glsl"))
        .addSource(rs.get("Blur.frag"));

    CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({vert, frag}));

    attachShaders({vert, frag});

    /* Older GLSL doesn't have gl_VertexID, vertices must be supplied explicitly */
    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isVersionSupported(Version::GL300))
    #else
    if(!Context::current()->isVersionSupported(Version::GLES300))
    #endif
    {
        bindAttributeLocation(Position::Location, "position");
    }

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::explicit_uniform_location>())
    #endif
    {
        #ifndef MAGNUM_TARGET_GLES
        if(!Context::current()->isVersionSupported(Version::GL300))
        #else
        if(!Context::current()->isVersionSupported(Version::GLES300))
        #endif
        {
            imageSizeInvertedUniform = uniformLocation("imageSizeInverted");
        }
    }

    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isExtensionSupported<Extensions::GL::ARB::shading_language_420pack>())
    #endif
    {
        setUniform(uniformLocation("textureData"), TextureLayer);
    }
}

Blur& Blur::setImageSizeInverted(const Vector2& size) {
    #ifndef MAGNUM_TARGET_GLES
    if(!Context::current()->isVersionSupported(Version::GL300))
    #else
    if(!Context::current()->isVersionSupported(Version::GLES300))
    #endif
    {
        setUniform(imageSizeInvertedUniform, size);
    }

    return *this;
}

Blur& Blur::setTexture(Texture2D& texture) {
    texture.bind(TextureLayer);
    return *this;
}

}}
