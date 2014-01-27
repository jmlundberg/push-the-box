#include <fstream>
#include <Corrade/Utility/Arguments.h>

#include "ResourceCompiler.h"

using namespace PushTheBox;

int main(int argc, char** argv) {
    Utility::Arguments args;
    args.addArgument("dae").setHelpKey("dae", "file.dae").setHelp("dae", "Input COLLADA file")
        .addArgument("conf").setHelpKey("conf", "file.conf").setHelp("conf", "Output mesh configuration file")
        .addArgument("mesh").setHelpKey("mesh", "file.mesh").setHelp("mesh", "Output mesh file")
        .setHelp("PushTheBox mesh compiler.")
        .parse(argc, argv);

    ResourceManagement::ResourceCompiler c(args.value("dae"));
    Utility::Configuration conf(args.value("conf"), Utility::Configuration::Flag::Truncate);
    std::ofstream out(args.value("mesh"));

    c.compileMeshes(&conf, out);

    return 0;
}
