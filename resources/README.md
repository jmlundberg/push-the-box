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

You need to have `magnum-fontconverter` utility, `FreeTypeFont` and
`MagnumFontConverter` plugins installed. See `magnum-fontconverter --help` for
more information. Run the utility in current working directory:

    magnum-font-converter --font FreeTypeFont --converter MagnumFontConverter --characters "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!:,. " --font-size 128 --atlas-size "2048 2048" --output-size "256 256" --radius 24 ../artwork/luckiest-guy.ttf luckiest-guy

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
