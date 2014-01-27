#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Arguments.h>
#include <Magnum/Platform/WindowlessGlxApplication.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/AbstractFontConverter.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Trade/AbstractImageConverter.h>

#include "PushTheBox.h"

#include "configure.h"
#include "resourceManagementConfigure.h"

namespace PushTheBox { namespace ResourceManagement {

class FontConverter: public Platform::WindowlessApplication {
    public:
        explicit FontConverter(const Arguments& arguments);

        int exec() override;

    private:
        Utility::Arguments args;
};

FontConverter::FontConverter(const Arguments& arguments): Platform::WindowlessApplication(arguments, nullptr) {
    args.addArgument("ttf").setHelpKey("ttf", "font.ttf").setHelp("ttf", "input font")
        .addArgument("output").setHelp("output", "output filename prefix")
        .addOption("characters", "abcdefghijklmnopqrstuvwxyz"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "0123456789?!:,. ").setHelp("characters", "characters to include in output")
        .addOption("font-size", "128").setHelpKey("font-size", "\"X Y\"").setHelp("font-size", "TTF font size")
        .addOption("atlas-size", "2048 2048").setHelpKey("atlas-size", "\"X Y\"").setHelp("atlas-size", "glyph atlas size")
        .addOption("output-size", "256 256").setHelpKey("output-size", "\"X Y\"").setHelp("output-size", "output atlas size")
        .addOption("radius", "24").setHelpKey("radius", "N").setHelp("radius", "distance field computation radius")
        .setHelp("Converts TTF font to distance-field Magnum raster font of given atlas size.\nOutput is saved to files output.tga and output.conf.")
        .parse(arguments.argc, arguments.argv);

    createContext();
}

int FontConverter::exec() {
    /* Font converter dependencies */
    PluginManager::Manager<Trade::AbstractImageConverter> imageConverterManager(MAGNUM_PLUGINS_IMAGECONVERTER_DIR);

    /* Load font */
    PluginManager::Manager<Text::AbstractFont> fontManager(MAGNUM_PLUGINS_FONT_DIR);
    Debug() << "Loading font plugin from" << MAGNUM_PLUGINS_FONT_DIR;
    Debug() << "    status:" << fontManager.load("FreeTypeFont");

    std::unique_ptr<Text::AbstractFont> font = fontManager.instance("FreeTypeFont");
    CORRADE_INTERNAL_ASSERT(font);

    /* Load font converter */
    PluginManager::Manager<Text::AbstractFontConverter> converterManager(MAGNUM_PLUGINS_FONTCONVERTER_DIR);
    Debug() << "Loading converter plugin from" << MAGNUM_PLUGINS_FONTCONVERTER_DIR;
    Debug() << "    status:" << converterManager.load("MagnumFontConverter");

    std::unique_ptr<Text::AbstractFontConverter> converter = converterManager.instance("MagnumFontConverter");
    CORRADE_INTERNAL_ASSERT(converter);

    Debug() << "Populating glyph cache...";

    /* Open font and fill distance field glyph cache */
    font->openFile(args.value("ttf"), args.value<Float>("font-size"));
    Text::DistanceFieldGlyphCache cache(args.value<Vector2i>("atlas-size"), args.value<Vector2i>("output-size"), args.value<Int>("radius"));
    font->fillGlyphCache(cache, args.value("characters"));

    Debug() << "Converting font...";

    /* Convert the font */
    converter->exportFontToFile(*font, cache, args.value("output"), args.value("characters"));

    Debug() << "Done.";

    return 0;
}

}}

MAGNUM_WINDOWLESSAPPLICATION_MAIN(PushTheBox::ResourceManagement::FontConverter)
