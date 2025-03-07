#include "engine.h"

#include <core/log.h>
#include <core/fs.h>
#include <event/event.h>
#include <plugins/loader.h>
#include <render/vulkan.h>
#include <render/render.h>
#include <scenegraph/scene_manager.h>
#include <utils/uuid.h>
#include <utils/thread_pool.h>
#include <chrono>

using namespace element;

static std::chrono::high_resolution_clock timer;
std::chrono::time_point<std::chrono::high_resolution_clock> current_time;

bool __detail::__engine_closed = false;
engine::settings_type engine::settings;

bool __detail::__engine_close_event_listener(events::close&) {
    __detail::__engine_closed = true;
    return true;
}

void engine::setup() {
    ELM_INFO("Element engine version {0} starting...", ELM_VERSION);
    ELM_INFO("Application {0} version {1}", settings.app_name, settings.app_version);
    ELM_INFO("Configuring application...");
    uuid::reseed_generator();
    thread_pool::init(8);
    vulkan::init_instance();
    fs::load_resources();
    ELM_INFO("Configuration done");
}

void engine::cleanup() {
    ELM_INFO("Application will close soon. Cleanning up...");
    render::reset_renderer();
    scenegraph::destroy_all_scenes();
    plugins::unload_all_plugins();
    render::cleanup_renderer();
    vulkan::cleanup();
    thread_pool::stop();
}

void engine::start() {
    ELM_INFO("Starting application...");
    current_time = timer.now();
}

void engine::tick() {
    auto new_time = timer.now();
    auto delta_time = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(new_time - current_time).count() / 1000.0;
    current_time = new_time;
    events::update u{delta_time};
    event_manager::send_event<events::update>(u);
}

void engine::stop() {
    
}

void engine::execute() {
    ELM_DEBUG("Applilcation execute called");
    start();
    while (!should_close()) tick();
    stop();
}

ELM_REGISTER_EVENT_CALLBACK(events::close, __detail::__engine_close_event_listener, event_callback_priority::lowest)