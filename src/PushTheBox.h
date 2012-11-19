#ifndef PushTheBox_PushTheBox_h
#define PushTheBox_PushTheBox_h

/** @file
 * @brief Basic definitions
 */

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

/** @brief Base 3D object */
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D<>> Object3D;

/** @brief Base 3D scene */
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D<>> Scene3D;

/** @brief Base 2D object */
typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D<>> Object2D;

/** @brief Base 2D scene */
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D<>> Scene2D;

/**
@brief Base for application

Magnum::Platform::Sdl2Application or Magnum::Platform::NaClApplication.
*/
#ifndef MAGNUM_TARGET_NACL
typedef Platform::Sdl2Application ApplicationBase;
#else
typedef Platform::NaClApplication ApplicationBase;
#endif

/** @brief Resource manager */
typedef ResourceManager<AbstractShaderProgram, Buffer, Mesh> SceneResourceManager;

}

#endif
