#include "Serialiser.h"
#include "Log.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace Sprocket {

void Serialiser::Serialise(const std::string& file)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << "Scene Name";
    out << YAML::EndMap;

    std::ofstream fout(file);
    fout << out.c_str();
}

void Serialiser::Deserialise(const std::string& file)
{
    SPKT_LOG_ERROR("Deserialising not yet implemented");
    // TODO
}

}