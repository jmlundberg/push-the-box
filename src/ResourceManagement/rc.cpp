#include <fstream>
#include <Corrade/Utility/Arguments.h>

#include "ResourceCompiler.h"

using namespace PushTheBox;

int main(int argc, char** argv) {
    Utility::Arguments args;
    args.addArgument("dae").setHelp("dae", "Input COLLADA file", "file.dae")
        .addArgument("conf").setHelp("conf", "Output mesh configuration file", "file.conf")
        .addArgument("mesh").setHelp("mesh", "Output mesh file", "file.mesh")
        .setHelp("PushTheBox mesh compiler.")
        .parse(argc, argv);

    ResourceManagement::ResourceCompiler c(args.value("dae"));
    Utility::Configuration conf(args.value("conf"), Utility::Configuration::Flag::Truncate);
    std::ofstream out(args.value("mesh"));

    c.compileMeshes(&conf, out);

    return 0;
}
