#ifndef PushTheBox_Floor_h
#define PushTheBox_Floor_h

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox {

class Floor: public Object3D, SceneGraph::Drawable<3> {
    public:
        Floor(const Color3<GLfloat>& color, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

    private:
        Color3<GLfloat> color;
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Buffer> buffer;
        Resource<Mesh> mesh;
};

}

#endif
