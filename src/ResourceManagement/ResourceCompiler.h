#ifndef PushTheBox_ResourceManagement_ResourceCompiler_h
#define PushTheBox_ResourceManagement_ResourceCompiler_h

#include <iosfwd>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>

#include "PushTheBox.h"

namespace Corrade { namespace Utility {
    class ConfigurationGroup;
}}

namespace PushTheBox { namespace ResourceManagement {

class ResourceCompiler {
    public:
        ResourceCompiler(const std::string& filename);

        void compileMeshes(Utility::ConfigurationGroup* configuration, std::ostream& out);

    private:
        PluginManager::Manager<Trade::AbstractImporter> manager;
        std::unique_ptr<Trade::AbstractImporter> importer;
};

}}

#endif
