#include "pandaFramework.h"
#include "pandaSystem.h"
#include "windowFramework.h"
#include "nodePath.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include "load_prc_file.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "argstack.h"

static WindowFramework *window = nullptr;
static NodePath render;
static NodePath *model  = nullptr;
static ArgStack* stack =  nullptr;

static PandaFramework g_framework;  // global instance
using Dispatcher = std::function<void*(void*, void**, int)>;
std::unordered_map<std::string, Dispatcher> registry;

extern "C" {

    // Register functions (note, registry functions CANNOT return be used to return a primative, else memory issues)
    void init_registry() {
        registry["open_window"] = [](void* obj, void** args, int argc) -> void* {
            if (argc < 2) return nullptr;
            auto w = static_cast<WindowFramework*>(obj);
            int x = *static_cast<int*>(args[0]);
            int y = *static_cast<int*>(args[1]);
            int result = open_window(w, x, y);

            // allocate return value on heap (caller must free)
            //~ return new int(result);
            return nullptr;
        };

        registry["get_title"] = [](void* obj, void** args, int argc) -> void* {
            auto w = static_cast<WindowFramework*>(obj);
            const char* result = get_title(w);
            return (void*)result; // returning pointer directly, caller should not free
        };

        registry["close_window"] = [](void* obj, void** args, int argc) -> void* {
            auto w = static_cast<WindowFramework*>(obj);
            close_window(w);
            return nullptr; // void return
        };
    }

    // Call by string name, handles some overloads
    void* call_func(const std::string& name, void* obj, void** args, int argc) {
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second(obj, args, argc);
        } else {
            std::cerr << "No such function: " << name << "\n";
            return nullptr;
        }
    }
    
    
    
    
    //- load_prc_file.h
    void p_load_prc_file(const char *filename) {
        load_prc_file(Filename(filename));
    }

    void* p_load_prc_file_data(const char *name, const char *data) {
        ConfigPage *page = load_prc_file_data(std::string(name), std::string(data));
        return (void*)page; 
    }

    void p_unload_prc_file(void *page_ptr) {
        ConfigPage *page = static_cast<ConfigPage*>(page_ptr);
        unload_prc_file(page);
    }

    void p_hash_prc_variables(void *hash_ptr) {
        // Expect caller to pass an allocated HashVal
        HashVal *hash = static_cast<HashVal*>(hash_ptr);
        hash_prc_variables(*hash);
    }
    
    //- pandaFramework.h
    // Constructor wrapper (creates and returns pointer to new framework)
    PandaFramework* p_PandaFramework() {
        return new PandaFramework();
    }

    // Destructor wrapper
    void p_delete_PandaFramework(PandaFramework* fw) {
        delete fw;
    }

    void p_open_framework() {
        g_framework.open_framework();
    }

    void p_open_framework_with_args(int argc, char **argv) {
        g_framework.open_framework(argc, argv);
    }

    void p_close_framework() {
        g_framework.close_framework();
    }

    GraphicsPipe* p_get_default_pipe() {
        return g_framework.get_default_pipe();
    }

    GraphicsEngine* p_get_graphics_engine() {
        return g_framework.get_graphics_engine();
    }

    WindowFramework* p_open_window() {
        return g_framework.open_window();
    }

    void p_set_window_title(const char* title) {
        g_framework.set_window_title(std::string(title));
    }

    NodePath* p_get_models() {
        // return by pointer so FFI can handle it
        return new NodePath(g_framework.get_models());
    }

    void p_main_loop() {
        g_framework.main_loop();
    }

    void p_set_exit_flag() {
        g_framework.set_exit_flag();
    }

    void p_clear_exit_flag() {
        g_framework.clear_exit_flag();
    }

    bool p_all_windows_closed() {
        return g_framework.all_windows_closed();
    }

    // Example static passthrough
    int p_hide_collision_solids(NodePath* node) {
        return PandaFramework::hide_collision_solids(*node);
    }

    int p_show_collision_solids(NodePath* node) {
        return PandaFramework::show_collision_solids(*node);
    }

    // Example task callback passthrough
    AsyncTask::DoneStatus p_task_data_loop(GenericAsyncTask* task, void* data) {
        return PandaFramework::task_data_loop(task, data);
    }
    
    //
    
} // extern "C"



    //~ // Initialize Panda3D and open a window
    //~ void* panda_init(int width, int height) {
        //~ load_prc_file_data("", "load-display pandagl");
        //~ framework.open_framework();
        //~ framework.set_window_title("Panda3D from TypeScript!");
        //~ window = framework.open_window();
        //~ render = window->get_render();
        //~ return (void*)window;
    //~ }

    //~ // allocate on heap so pointer stays valid
    //~ void* panda_load_model(const char* filename) {
        //~ NodePath* model = new NodePath(window->load_model(window->get_panda_framework()->get_models(), filename));
        //~ model->reparent_to(render);
        //~ return (void*)model;
    //~ }

    //~ void panda_rotate_model(void* model_ptr, float h, float p, float r) {
        //~ NodePath* model = static_cast<NodePath*>(model_ptr);
        //~ model->set_hpr(h, p, r);
    //~ }

    //~ // Run main loop (blocking)
    //~ void panda_run() {
        //~ framework.main_loop();
    //~ }
    
    //~ // Run main loop (blocking)
    //~ void panda_run() {
        //~ framework.main_loop();
    //~ }

    //~ // Shutdown
    //~ void panda_shutdown() {
        //~ framework.close_framework();
    //~ }


// panda_wrapper.cpp

//~ extern "C" void panda_load_model(const char* filename) {
    //~ g_model = window->load_model(framework.get_models(), filename);
    //~ g_model.reparent_to(window->get_render());
//~ }

//~ extern "C" void* panda_load_model(const char* filename) {
    //~ NodePath* model = new NodePath(window->load_model(loader, filename));
    //~ model->reparent_to(window->get_render());
    //~ return model;
//~ }


//~ // Load a model and attach to render
//~ void* panda_load_model(const char* path) {
    //~ if (!window) return nullptr;
    //~ model = window->load_model(framework.get_models(), path);
    //~ model.reparent_to(window->get_render());
    //~ return (void*)&model;
//~ }

//~ // Rotate the model
//~ void panda_rotate_model(float h, float p, float r) {
    //~ if (!model.is_empty()) {
        //~ model.set_hpr(model.get_h() + h, model.get_p() + p, model.get_r() + r);
    //~ }
//~ }
