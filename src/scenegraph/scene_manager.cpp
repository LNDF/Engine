#include "scene_manager.h"

#include <unordered_map>
#include <utility>
#include <core/fs.h>
#include <serialization/scenegraph.h>

static std::unordered_map<element::uuid, element::scenegraph::scene> scene_map;

using namespace element;

void scenegraph::load_scene(const uuid& id) {
    auto input = fs::get_resource(id);
    scene s;
    binary_deserializer deserialize = create_binary_deserializer(*input);
    deserialize(s);
    import_scene(id, std::move(s));
    
}

void scenegraph::import_scene(const uuid& id, scene&& s) {
    scene_map.insert(std::make_pair(id, std::move(s))).first->second.init_scene(id);
}

scenegraph::scene* scenegraph::get_scene(const uuid& id) {
    auto it = scene_map.find(id);
    if (it == scene_map.end()) {
        return nullptr;
    }
    return &it->second;
}

void scenegraph::destroy_scene(const uuid& id) {
    scene_map.erase(id);
}