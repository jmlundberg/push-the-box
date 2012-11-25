#ifndef PushTheBox_ResourceManagement_ResourceCompiler_h
#define PushTheBox_ResourceManagement_ResourceCompiler_h

#include <iosfwd>
#include <PluginManager/PluginManager.h>
#include <Trade/AbstractImporter.h>

#include "PushTheBox.h"

namespace Corrade { namespace Utility {
    class ConfigurationGroup;
}}

namespace PushTheBox { namespace ResourceManagement {

class ResourceCompiler {
    public:
        ResourceCompiler(const std::string& filename);

        void compileMeshes(Corrade::Utility::ConfigurationGroup* configuration, std::ostream& out);

    private:
        Corrade::PluginManager::PluginManager<Trade::AbstractImporter> manager;
        Trade::AbstractImporter* importer;
};

}}

#endif
