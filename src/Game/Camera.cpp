#include "Camera.h"

#include <DefaultFramebuffer.h>
#include <Renderbuffer.h>
#include <RenderbufferFormat.h>
#include <Renderer.h>
#include <TextureFormat.h>

namespace PushTheBox { namespace Game {

Camera::Camera(Object3D* parent): Object3D(parent), SceneGraph::Camera3D(this), _blurred(true), multisampleFramebuffer({{}, defaultFramebuffer.viewport().size()/8}), framebuffer(multisampleFramebuffer.viewport()), blurShaderHorizontal(Shaders::Blur::Direction::Horizontal), blurShaderVertical(Shaders::Blur::Direction::Vertical) {
    setPerspective(Deg(35.0f), 1.0f, 0.001f, 100.0f);
    setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /** @todo some option to disable this */
    if(1) {
        color.setStorageMultisample(16, RenderbufferFormat::RGBA8, multisampleFramebuffer.viewport().size());
        depth.setStorageMultisample(16, RenderbufferFormat::DepthComponent24, multisampleFramebuffer.viewport().size());
    } else {
        color.setStorage(RenderbufferFormat::RGBA8, multisampleFramebuffer.viewport().size());
        depth.setStorage(RenderbufferFormat::DepthComponent24, multisampleFramebuffer.viewport().size());
    }
    multisampleFramebuffer.attachRenderbuffer(Framebuffer::ColorAttachment(0), &color);
    multisampleFramebuffer.attachRenderbuffer(Framebuffer::BufferAttachment::Depth, &depth);

    texture1.setStorage(1, TextureFormat::RGB8, multisampleFramebuffer.viewport().size())
        ->setMagnificationFilter(Sampler::Filter::Linear)
        ->setMinificationFilter(Sampler::Filter::Nearest)
        ->setWrapping(Sampler::Wrapping::ClampToEdge);
    texture2.setStorage(1, TextureFormat::RGB8, multisampleFramebuffer.viewport().size())
        ->setMagnificationFilter(Sampler::Filter::Linear)
        ->setMinificationFilter(Sampler::Filter::Nearest)
        ->setWrapping(Sampler::Wrapping::ClampToEdge);
    framebuffer.attachTexture2D(Framebuffer::ColorAttachment(0), &texture1, 0);
    framebuffer.attachTexture2D(Framebuffer::ColorAttachment(1), &texture2, 0);

    fullScreenTriangle.setPrimitive(Mesh::Primitive::TriangleStrip)
        ->setVertexCount(4);
}

void Camera::setViewport(const Vector2i& size) {
    SceneGraph::Camera3D::setViewport(size/8);

    multisampleFramebuffer.setViewport({{}, size/8});
    framebuffer.setViewport({{}, size/8});
    /** @todo resize also texture */
}

void Camera::draw(SceneGraph::DrawableGroup3D& group) {
    if(!_blurred) {
        SceneGraph::Camera3D::draw(group);
        return;
    }

    /* Draw scene to (multisampled) default framebuffer */
    multisampleFramebuffer.bind(FramebufferTarget::Draw);
    multisampleFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);
    SceneGraph::Camera3D::draw(group);

    /* Resolve to first texture */
    framebuffer.mapForDraw(Framebuffer::ColorAttachment(0));
    Framebuffer::blit(multisampleFramebuffer, framebuffer, multisampleFramebuffer.viewport(), FramebufferBlit::ColorBuffer);

    /* Blur first texture horizontally to second one */
    framebuffer.mapForDraw(Framebuffer::ColorAttachment(1));
    blurShaderHorizontal.use();
    texture1.bind(Shaders::Blur::TextureLayer);
    fullScreenTriangle.draw();

    /* Blur second texture vertically to screen FB */
    defaultFramebuffer.bind(FramebufferTarget::Draw);
    defaultFramebuffer.clear(FramebufferClear::Depth);
    blurShaderVertical.use();
    texture2.bind(Shaders::Blur::TextureLayer);
    fullScreenTriangle.draw();
}

}}
