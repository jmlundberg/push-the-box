#include "Camera.h"

#include <Magnum/Buffer.h>
#include <Magnum/Context.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Extensions.h>
#include <Magnum/Mesh.h>
#include <Magnum/Renderbuffer.h>
#include <Magnum/RenderbufferFormat.h>
#include <Magnum/Renderer.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/TextureFormat.h>

namespace PushTheBox { namespace Game {

Camera::Camera(Object3D* parent): Object3D(parent), SceneGraph::Camera3D(*this), _blurred(true), multisampleFramebuffer({{}, defaultFramebuffer.viewport().size()/8}), framebuffer1(multisampleFramebuffer.viewport()), framebuffer2(multisampleFramebuffer.viewport()), blurShaderHorizontal(Shaders::Blur::Direction::Horizontal), blurShaderVertical(Shaders::Blur::Direction::Vertical) {
    /* Get full screen triangle */
    fullScreenTriangleBuffer = SceneResourceManager::instance().get<Buffer>("fullscreentriangle");
    fullScreenTriangle = SceneResourceManager::instance().get<Mesh>("fullscreentriangle");

    setPerspective(Deg(35.0f), 1.0f, 0.001f, 100.0f);
    setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);
    SceneGraph::Camera3D::setViewport(defaultFramebuffer.viewport().size());

    /* Decide about multisampling on ES2 */
    #ifdef MAGNUM_TARGET_GLES2
    if(Context::current()->isExtensionSupported<Extensions::GL::ANGLE::framebuffer_multisample>() ||
       Context::current()->isExtensionSupported<Extensions::GL::NV::framebuffer_multisample>()) {
        _multisample = true;
    } else {
        Debug() << "Neither" << Extensions::GL::ANGLE::framebuffer_multisample::string()
                << "nor" << Extensions::GL::NV::framebuffer_multisample::string()
                << "is supported, expect ugly blur :-)";
        _multisample = false;
    }
    #else
    _multisample = true;
    #endif

    /* Configure depth buffer */
    RenderbufferFormat depthFormat = RenderbufferFormat::DepthComponent24;
    #ifdef MAGNUM_TARGET_GLES2
    if(!Context::current()->isExtensionSupported<Extensions::GL::OES::depth24>()) {
        Debug() << Extensions::GL::OES::depth24::string() << "not supported, fallback to 16bit depth buffer";
        depthFormat = RenderbufferFormat::DepthComponent16;
    }
    #endif
    depth.setStorage(depthFormat, multisampleFramebuffer.viewport().size());
    framebuffer1.attachRenderbuffer(Framebuffer::BufferAttachment::Depth, depth);
    framebuffer2.attachRenderbuffer(Framebuffer::BufferAttachment::Depth, depth);

    /* Configure multisample framebuffer */
    if(_multisample) {
        #ifdef MAGNUM_TARGET_GLES
        MAGNUM_ASSERT_EXTENSION_SUPPORTED(Extensions::GL::OES::rgb8_rgba8);
        #endif
        multisampleColor.setStorageMultisample(16, RenderbufferFormat::RGBA8, multisampleFramebuffer.viewport().size());
        multsampleDepth.setStorageMultisample(16, depthFormat, multisampleFramebuffer.viewport().size());
        multisampleFramebuffer.attachRenderbuffer(Framebuffer::ColorAttachment(0), multisampleColor);
        multisampleFramebuffer.attachRenderbuffer(Framebuffer::BufferAttachment::Depth, multsampleDepth);
        CORRADE_INTERNAL_ASSERT(multisampleFramebuffer.checkStatus(FramebufferTarget::ReadDraw) == Framebuffer::Status::Complete);
    }

    /* Configure textures */
    TextureFormat internalFormat = TextureFormat::RGB8;
    #ifdef MAGNUM_TARGET_GLES
    if(!Context::current()->isExtensionSupported<Extensions::GL::OES::required_internalformat>())
        internalFormat = TextureFormat::RGB;
    #endif
    texture1.setStorage(1, internalFormat, multisampleFramebuffer.viewport().size())
        .setMagnificationFilter(Sampler::Filter::Linear)
        .setMinificationFilter(Sampler::Filter::Nearest)
        .setWrapping(Sampler::Wrapping::ClampToEdge);
    texture2.setStorage(1, internalFormat, multisampleFramebuffer.viewport().size())
        .setMagnificationFilter(Sampler::Filter::Linear)
        .setMinificationFilter(Sampler::Filter::Nearest)
        .setWrapping(Sampler::Wrapping::ClampToEdge);
    framebuffer1.attachTexture(Framebuffer::ColorAttachment(0), texture1, 0);
    framebuffer2.attachTexture(Framebuffer::ColorAttachment(0), texture2, 0);

    /* Verify that everything is sane */
    CORRADE_INTERNAL_ASSERT(framebuffer1.checkStatus(FramebufferTarget::ReadDraw) == Framebuffer::Status::Complete);
    CORRADE_INTERNAL_ASSERT(framebuffer2.checkStatus(FramebufferTarget::ReadDraw) == Framebuffer::Status::Complete);
}

void Camera::setViewport(const Vector2i& size) {
    SceneGraph::Camera3D::setViewport(size);

    multisampleFramebuffer.setViewport({{}, size/8});
    framebuffer1.setViewport({{}, size/8});
    framebuffer2.setViewport({{}, size/8});
    /** @todo resize also texture */

    blurShaderHorizontal.setImageSizeInverted(8.0f/Vector2(size));
    blurShaderVertical.setImageSizeInverted(8.0f/Vector2(size));
}

void Camera::draw(SceneGraph::DrawableGroup3D& group) {
    /* Render the scene normally */
    if(!_blurred) {
        defaultFramebuffer.bind(FramebufferTarget::ReadDraw);
        SceneGraph::Camera3D::draw(group);
        return;
    }

    /* Draw scene to multisampled framebuffer */
    if(_multisample) {
        multisampleFramebuffer.bind(FramebufferTarget::Draw);
        multisampleFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);
        SceneGraph::Camera3D::draw(group);

        /* Resolve to first texture */
        Framebuffer::blit(multisampleFramebuffer, framebuffer1, multisampleFramebuffer.viewport(), FramebufferBlit::ColorBuffer);

    /* Single sample fallback */
    } else {
        framebuffer1.bind(FramebufferTarget::ReadDraw);
        framebuffer1.clear(FramebufferClear::Color|FramebufferClear::Depth);
        SceneGraph::Camera3D::draw(group);
    }

    /* Blur first texture horizontally to second one */
    framebuffer2.bind(FramebufferTarget::ReadDraw);
    framebuffer2.clear(FramebufferClear::Depth);
    blurShaderHorizontal.use();
    texture1.bind(Shaders::Blur::TextureLayer);
    fullScreenTriangle->draw();

    /* Blur second texture vertically to screen FB */
    defaultFramebuffer.bind(FramebufferTarget::ReadDraw);
    defaultFramebuffer.clear(FramebufferClear::Depth);
    blurShaderVertical.use();
    texture2.bind(Shaders::Blur::TextureLayer);
    fullScreenTriangle->draw();
}

}}
