#include "Splash.h"

#include <Magnum/Buffer.h>
#include <Magnum/Context.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Extensions.h>
#include <Magnum/Mesh.h>
#include <Magnum/Renderer.h>
#include <Magnum/Texture.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/SceneGraph/Camera2D.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData2D.h>

#include "Application.h"
#include "Game/Game.h"

namespace PushTheBox { namespace Splash {

Splash::Splash() {
    /* Configure camera */
    camera = new SceneGraph::Camera2D(scene);
    camera->setProjection({8.0f/3.0f, 2.0f})
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setViewport(defaultFramebuffer.viewport().size());

    /* Add splash image */
    class Image: public Object2D, SceneGraph::Drawable2D {
        public:
            Image(Object2D* parent, SceneGraph::DrawableGroup2D* drawableGroup): Object2D(parent), SceneGraph::Drawable2D(*this, drawableGroup) {
                /* Shader */
                shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::DistanceFieldVector2D>("text2d");

                /* Mesh */
                Trade::MeshData2D data = Primitives::Square::solid(Primitives::Square::TextureCoords::Generate);
                MeshTools::transformVectorsInPlace(Matrix3::scaling(Vector2::xScale(4.0f/3.0f)), data.positions(0));
                vertices.setData(MeshTools::interleave(data.positions(0), data.textureCoords2D(0)), BufferUsage::StaticDraw);
                mesh.setPrimitive(data.primitive())
                    .setCount(data.positions(0).size())
                    .addVertexBuffer(vertices, 0, Shaders::DistanceFieldVector2D::Position(), Shaders::DistanceFieldVector2D::TextureCoordinates());

                /* Texture image */
                Utility::Resource rs("PushTheBoxResources");
                auto importer = SceneResourceManager::instance().get<Trade::AbstractImporter>("tga-importer");
                CORRADE_INTERNAL_ASSERT_OUTPUT(importer->openData(rs.getRaw("splash.tga")));
                std::optional<Trade::ImageData2D> image = importer->image2D(0);
                CORRADE_INTERNAL_ASSERT(image);

                /* Decide about the format */
                #ifndef MAGNUM_TARGET_GLES
                MAGNUM_ASSERT_EXTENSION_SUPPORTED(Extensions::GL::ARB::texture_rg);
                #endif
                #if !defined(MAGNUM_TARGET_GLES) || defined(MAGNUM_TARGET_GLES3)
                const TextureFormat internalFormat = TextureFormat::R8;
                #else
                const TextureFormat internalFormat =
                    Context::current()->isExtensionSupported<Extensions::GL::EXT::texture_rg>() ?
                    TextureFormat::Red : TextureFormat::Luminance;
                #endif

                texture.setMinificationFilter(Sampler::Filter::Linear)
                    .setMagnificationFilter(Sampler::Filter::Linear)
                    .setWrapping(Sampler::Wrapping::ClampToEdge)
                    .setImage(0, internalFormat, *image);
            }

        protected:
            void draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D& camera) override {
                shader->setColor(Color3::fromHSV(Deg(210.0f), 0.55f, 0.9f))
                    .setOutlineColor(Color3(1.0f))
                    .setOutlineRange(0.55f, 0.425f)
                    .setTransformationProjectionMatrix(camera.projectionMatrix()*transformationMatrix)
                    .setVectorTexture(texture);

                mesh.draw(*shader);
            }

        private:
            Resource<AbstractShaderProgram, Shaders::DistanceFieldVector2D> shader;
            Texture2D texture;
            Buffer vertices;
            Mesh mesh;
    };

    new Image(&scene, &drawables);
}

void Splash::focusEvent() {
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Splash::blurEvent() {
    setPropagatedEvents({});
}

void Splash::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
}

void Splash::drawEvent() {
    Renderer::setFeature(Renderer::Feature::Blending, true);
    Renderer::setBlendFunction(Renderer::BlendFunction::One, Renderer::BlendFunction::OneMinusSourceAlpha);
    Renderer::setFeature(Renderer::Feature::DepthTest, false);
    camera->draw(drawables);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setFeature(Renderer::Feature::Blending, false);
}

void Splash::mouseReleaseEvent(MouseEvent&) {
    Application::instance()->focusScreen(*Application::instance()->gameScreen());
}

}}
