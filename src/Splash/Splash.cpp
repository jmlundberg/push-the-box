#include "Splash.h"

#include <Context.h>
#include <DefaultFramebuffer.h>
#include <Extensions.h>
#include <Renderer.h>
#include <Texture.h>
#include <TextureFormat.h>
#include <MeshTools/Interleave.h>
#include <MeshTools/Transform.h>
#include <Primitives/Square.h>
#include <SceneGraph/Camera2D.h>
#include <Shaders/DistanceFieldVector.h>
#include <Trade/AbstractImporter.h>
#include <Trade/ImageData.h>
#include <Trade/MeshData2D.h>

#include "Application.h"
#include "Game/Game.h"

namespace PushTheBox { namespace Splash {

Splash::Splash() {
    /* Configure camera */
    camera = new SceneGraph::Camera2D(scene);
    camera->setProjection({8.0f/3.0f, 2.0f})
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Add splash image */
    class Image: public Object2D, SceneGraph::Drawable2D {
        public:
            Image(Object2D* parent, SceneGraph::DrawableGroup2D* drawableGroup): Object2D(parent), SceneGraph::Drawable2D(*this, drawableGroup) {
                /* Shader */
                shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::DistanceFieldVector2D>("text2d");

                /* Mesh */
                Trade::MeshData2D data = Primitives::Square::solid(Primitives::Square::TextureCoords::Generate);
                MeshTools::transformVectorsInPlace(Matrix3::scaling(Vector2::xScale(4.0f/3.0f)), data.positions(0));
                MeshTools::interleave(mesh, vertices, Buffer::Usage::StaticDraw, data.positions(0), data.textureCoords2D(0));
                mesh.setPrimitive(data.primitive())
                    .addInterleavedVertexBuffer(vertices, 0, Shaders::DistanceFieldVector2D::Position(), Shaders::DistanceFieldVector2D::TextureCoordinates());

                /* Texture image */
                Utility::Resource rs("PushTheBoxResources");
                auto importer = SceneResourceManager::instance().get<Trade::AbstractImporter>("tga-importer");
                CORRADE_INTERNAL_ASSERT_OUTPUT(importer->openData(rs.getRaw("splash.tga")));
                Trade::ImageData2D* image = importer->image2D(0);
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
                texture.bind(Shaders::DistanceFieldVector2D::VectorTextureLayer);

                shader->setColor(Color3::fromHSV(Deg(210.0f), 0.55f, 0.9f))
                    .setOutlineColor(Color3(1.0f))
                    .setOutlineRange(0.55f, 0.425f)
                    .setTransformationProjectionMatrix(camera.projectionMatrix()*transformationMatrix)
                    .use();

                mesh.draw();
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

void Splash::mouseReleaseEvent(AbstractScreen::MouseEvent&) {
    Application::instance()->focusScreen(Application::instance()->gameScreen());
}

}}
