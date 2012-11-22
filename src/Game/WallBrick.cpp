#include "WallBrick.h"

#include <Buffer.h>
#include <Mesh.h>
#include <MeshTools/Interleave.h>
#include <MeshTools/Transform.h>
#include <Primitives/Cube.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

WallBrick::WallBrick(const Color3<>& color, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), color(color) {
    /* Get shader and buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    buffer = SceneResourceManager::instance()->get<Buffer>("cube");

    /* Create cube mesh, if not already exists */
    if(!(mesh = SceneResourceManager::instance()->get<Mesh>("cube"))) {
        SceneResourceManager::instance()->set<Buffer>(buffer.key(),
            new Buffer, ResourceDataState::Final, ResourcePolicy::ReferenceCounted);
        SceneResourceManager::instance()->set<Mesh>(mesh.key(),
            new Mesh, ResourceDataState::Final, ResourcePolicy::Manual);

        Primitives::Cube cube;
        MeshTools::transform(Matrix4::scaling(Vector3(0.5f))*Matrix4::translation(Vector3::yAxis(1.0f)), *cube.positions(0));
        MeshTools::interleave(mesh, buffer, Buffer::Usage::StaticDraw, *cube.positions(0), *cube.normals(0));
        mesh->setPrimitive(cube.primitive())
            ->addInterleavedVertexBuffer(buffer, 0, Shaders::PhongShader::Position(), Shaders::PhongShader::Normal());
    }
}

void WallBrick::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformation(transformationMatrix)
          ->setProjection(camera->projectionMatrix())
          ->setDiffuseColor(color)
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
