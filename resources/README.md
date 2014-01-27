CREATING COMPILED RESOURCES
===========================

Compiling meshes
----------------

You need to have `ColladaImporter` plugin installed and `push-the-box-rc`
executable compiled. See `./push-the-box-rc --help` for more information.
Export COLLADA file from `artwork/models.blend` and then run the utility in
current working directory:

    ../build/src/ResourceManagement/push-the-box-rc ../artwork/models.dae push-the-box.conf push-the-box.mesh

Compiling font
--------------

You need to have `FreeTypeFont` and `MagnumFontConverter` plugins installed and
`push-the-box-font-converter` executable compiled. See
`./push-the-box-font-converter --help` for more information. Run the utility in
current working directory:

    ../build/src/ResourceManagement/push-the-box-font-converter ../artwork/luckiest-guy.ttf luckiest-guy

Converting splash to distance-field
-----------------------------------

You need to have `magnum-distancefieldconverter` utility, `PngImporter` and
`TgaImageConverter` plugins installed. Export `artwork/splash.svg` with black
background into PNG at 360 DPI (2560x1920) and convert it with imagemagick to
grayscale, e.g.:

    mogrify -type grayscale splash.png

Then call `magnum-distancefieldconverter` on it. See
`magnum-distancefieldconverter --help` for more information. Run the utility in
current working directory:

    magnum-distancefieldconverter --importer PngImporter --output-size "320 240" --radius 24 ../artwork/splash.png splash.tga
