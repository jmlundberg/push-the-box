#ifndef PushTheBox_PushTheBox_h
#define PushTheBox_PushTheBox_h

#include <Magnum.h>
#include <SceneGraph/SceneGraph.h>

namespace PushTheBox {

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D<>> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D<>> Scene3D;

}

#endif
