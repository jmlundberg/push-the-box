#include "ResourceCompiler.h"

#include <fstream>

using namespace PushTheBox;

int main(int argc, char** argv) {
    if(argc != 4) {
        Debug() << "Usage:" << argv[0] << "file.dae file.conf file.mesh";
        return 1;
    }

    ResourceManagement::ResourceCompiler c(argv[1]);
    Utility::Configuration conf(argv[2], Utility::Configuration::Flag::Truncate);
    std::ofstream out(argv[3]);

    c.compileMeshes(&conf, out);

    return 0;
}
