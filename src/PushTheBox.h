#ifndef PushTheBox_PushTheBox_h
#define PushTheBox_PushTheBox_h

/** @file
 * @brief Basic definitions
 */

#include <Magnum.h>
#include <SceneGraph/SceneGraph.h>
#include <Text/Text.h>

namespace PushTheBox {

using namespace Magnum;

/** @brief Base 3D object */
typedef SceneGraph::Object<SceneGraph::DualQuaternionTransformation> Object3D;

/** @brief Base 3D scene */
typedef SceneGraph::Scene<SceneGraph::DualQuaternionTransformation> Scene3D;

/** @brief Base 2D object */
typedef SceneGraph::Object<SceneGraph::DualComplexTransformation> Object2D;

/** @brief Base 2D scene */
typedef SceneGraph::Scene<SceneGraph::DualComplexTransformation> Scene2D;

/** @brief Resource manager */
typedef ResourceManager<AbstractShaderProgram, Buffer, Mesh, Text::AbstractFont, Text::GlyphCache> SceneResourceManager;

}

#endif
