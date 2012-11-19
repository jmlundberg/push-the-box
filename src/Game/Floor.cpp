#include "Floor.h"

#include <Math/Constants.h>
#include <MeshTools/Interleave.h>
#include <Primitives/Plane.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

Floor::Floor(const Color3<GLfloat>& color, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), color(color) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    buffer = SceneResourceManager::instance()->get<Buffer>("floor");

    /* Create floor mesh, if not already exists */
    if(!(mesh = SceneResourceManager::instance()->get<Mesh>("floor"))) {
        SceneResourceManager::instance()->set<Buffer>(buffer.key(),
            new Buffer, ResourceDataState::Final, ResourcePolicy::ReferenceCounted);
        SceneResourceManager::instance()->set<Mesh>(mesh.key(),
            new Mesh, ResourceDataState::Final, ResourcePolicy::Manual);

        Primitives::Plane plane;
        MeshTools::interleave(mesh, buffer, Buffer::Usage::StaticDraw, *plane.positions(0), *plane.normals(0));
        mesh->setPrimitive(plane.primitive())
            ->addInterleavedVertexBuffer(buffer, 0, Shaders::PhongShader::Position(), Shaders::PhongShader::Normal());
    }

    rotateX(deg(-90.0f));
    scale(Vector3(0.5f));
}

void Floor::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformation(transformationMatrix)
          ->setProjection(camera->projectionMatrix())
          ->setDiffuseColor(color)
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
