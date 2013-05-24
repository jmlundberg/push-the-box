#ifndef PushTheBox_ResourceManagement_ResourceCompiler_h
#define PushTheBox_ResourceManagement_ResourceCompiler_h

#include <iosfwd>
#include <PluginManager/Manager.h>
#include <Trade/AbstractImporter.h>

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
        Trade::AbstractImporter* importer;
};

}}

#endif
