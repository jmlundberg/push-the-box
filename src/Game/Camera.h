#ifndef PushTheBox_Game_Camera_h
#define PushTheBox_Game_Camera_h

#include <Magnum/Framebuffer.h>
#include <Magnum/Renderbuffer.h>
#include <Magnum/Resource.h>
#include <Magnum/Texture.h>
#include <Magnum/SceneGraph/Camera3D.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>

#include "PushTheBox.h"
#include "Shaders/Blur.h"

namespace PushTheBox { namespace Game {

class Camera: public Object3D, public SceneGraph::Camera3D {
    public:
        Camera(Object3D* parent = nullptr);

        void setViewport(const Vector2i& size) override;

        void draw(SceneGraph::DrawableGroup3D& group) override;

        inline void setBlurred(bool blurred) { _blurred = blurred; }

    private:
        bool _multisample, _blurred;

        Renderbuffer multisampleColor, multsampleDepth;
        Framebuffer multisampleFramebuffer;

        Texture2D texture1, texture2;
        Renderbuffer depth;
        Framebuffer framebuffer1, framebuffer2;

        Shaders::Blur blurShaderHorizontal;
        Shaders::Blur blurShaderVertical;
        Resource<Mesh> fullScreenTriangle;
        Resource<Buffer> fullScreenTriangleBuffer;
};

}}

#endif
