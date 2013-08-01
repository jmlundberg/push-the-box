#include <PluginManager/Manager.h>
#include <Platform/WindowlessGlxApplication.h>
#include <Text/AbstractFont.h>
#include <Text/AbstractFontConverter.h>
#include <Text/DistanceFieldGlyphCache.h>
#include <Trade/AbstractImageConverter.h>

#include "PushTheBox.h"

#include "configure.h"
#include "resourceManagementConfigure.h"

namespace PushTheBox { namespace ResourceManagement {

class FontConverter: public Platform::WindowlessApplication {
    public:
        explicit FontConverter(const Arguments& arguments);

        int exec() override;

    private:
        std::string ttfFilename,
            outputFilename;
};

FontConverter::FontConverter(const Arguments& arguments): Platform::WindowlessApplication(arguments) {
    if(arguments.argc != 3) {
        Debug() << "Usage:" << arguments.argv[0] << "font.ttf outputPrefix";
        std::exit(1);
    }

    ttfFilename = arguments.argv[1];
    outputFilename = arguments.argv[2];
}

int FontConverter::exec() {
    /* Font converter dependencies */
    PluginManager::Manager<Trade::AbstractImageConverter> imageConverterManager(MAGNUM_PLUGINS_IMAGECONVERTER_DIR);

    /* Load font */
    PluginManager::Manager<Text::AbstractFont> fontManager(MAGNUM_PLUGINS_FONT_DIR);
    Debug() << "Loading font plugin from" << MAGNUM_PLUGINS_FONT_DIR;
    Debug() << "    status:" << fontManager.load("FreeTypeFont");

    Text::AbstractFont* font = fontManager.instance("FreeTypeFont");
    CORRADE_INTERNAL_ASSERT(font);

    /* Load font converter */
    PluginManager::Manager<Text::AbstractFontConverter> converterManager(MAGNUM_PLUGINS_FONTCONVERTER_DIR);
    Debug() << "Loading converter plugin from" << MAGNUM_PLUGINS_FONTCONVERTER_DIR;
    Debug() << "    status:" << converterManager.load("MagnumFontConverter");

    Text::AbstractFontConverter* converter = converterManager.instance("MagnumFontConverter");
    CORRADE_INTERNAL_ASSERT(converter);

    Debug() << "Populating glyph cache...";

    constexpr const char characters[] = "abcdefghijklmnopqrstuvwxyz"
                                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "0123456789?!:,. ";

    /* Open font and fill distance field glyph cache */
    font->openFile(ttfFilename, 128.0f);
    Text::DistanceFieldGlyphCache cache(Vector2i(2048), Vector2i(256), 24);
    font->fillGlyphCache(cache, characters);

    Debug() << "Converting font...";

    /* Convert the font */
    converter->exportFontToFile(*font, cache, outputFilename, characters);

    Debug() << "Done.";

    delete font;
    delete converter;
    return 0;
}

}}

MAGNUM_WINDOWLESSAPPLICATION_MAIN(PushTheBox::ResourceManagement::FontConverter)
