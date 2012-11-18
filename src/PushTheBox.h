#ifndef PushTheBox_PushTheBox_h
#define PushTheBox_PushTheBox_h

#include <Magnum.h>
#include <SceneGraph/SceneGraph.h>

namespace Magnum { namespace Platform {
    #ifndef MAGNUM_TARGET_NACL
    class Sdl2Application;
    #else
    class NaClApplication;
    #endif
}}

namespace PushTheBox {

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D<>> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D<>> Scene3D;

#ifndef MAGNUM_TARGET_NACL
typedef Platform::Sdl2Application ApplicationBase;
#else
typedef Platform::NaClApplication ApplicationBase;
#endif

typedef ResourceManager<AbstractShaderProgram, Buffer, Mesh> SceneResourceManager;

}

#endif
