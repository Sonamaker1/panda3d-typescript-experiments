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
#include "arg_buffer.h"


#include <unordered_map>
#include <string>
#include <functional>


static WindowFramework *window = nullptr;
static NodePath render;
static NodePath *model  = nullptr;
static ArgBuffer* buf =  argbuffer_create();
static ArgBuffer* returnBuf =  argbuffer_create();
//~ static ArgBuffer* variableBuf =  argbuffer_create(); //will implement later.

//global object 
static PandaFramework g_framework;  // global instance

// Dispatcher signature
using Dispatcher = std::function<void*(ArgBuffer* args, int argc)>;
using DispatcherSTRING = std::function<void*(ArgBuffer* args, int argc)>;

// Registry mapping string -> dispatcher
static std::unordered_map<std::string, Dispatcher> registry;
static std::unordered_map_string_functions<std::string, DispatcherSTRING> registrySTRING;

// For functions returning something
#define REGISTER_FUNC(name, callable, rettype, ...) \
    registry[#name] = [](ArgBuffer* args, int argc) -> void* { \
        rettype result = (callable)(__VA_ARGS__); \
        return new rettype(result); \
    };

// For functions returning std::string
#define REGISTER_STRING(name, callable, rettype, ...) \
    registrySTRING[#name] = [](ArgBuffer* args, int argc) -> std::string { \
        rettype result = (callable)(__VA_ARGS__); \
        return new rettype(result); \
    };

// For void-returning functions
#define REGISTER_VOID_FUNC(name, callable, ...) \
    registry[#name] = [](ArgBuffer* args, int argc) -> void* { \
        (callable)(__VA_ARGS__); \
        return nullptr; \
    };
    
#define REGISTER(name, expected) \
    registry[name] = [](ArgBuffer* args, int argc) -> void* { \
        if (argc != expected) { \
            std::cerr << name << " expects " << expected << " arguments\n"; \
            return nullptr; \
        }

#define END_REGISTER return nullptr; \
        };
extern "C" {

    void init_registry() {
        
        // Argument type tokens
        #define b_bool     argbuffer_get_bool
        #define b_char     argbuffer_get_char
        #define b_cstring  argbuffer_get_cstring
        #define b_f32      argbuffer_get_f32
        #define b_f64      argbuffer_get_f64
        #define b_function argbuffer_get_function
        #define b_i8       argbuffer_get_i8
        #define b_i16      argbuffer_get_i16
        #define b_i32      argbuffer_get_i32
        #define b_i64      argbuffer_get_i64
        #define b_ptr      argbuffer_get_ptr
        #define b_u8       argbuffer_get_u8 
        #define b_u16      argbuffer_get_u16
        #define b_u32      argbuffer_get_u32
        #define b_u64      argbuffer_get_u64

        REGISTER_STRING(get_version_string, PandaSystem::get_global_ptr()->get_version_string, std::string);
        REGISTER_STRING(get_distributor, PandaSystem::get_global_ptr()->get_distributor, std::string);
        REGISTER_STRING(get_compiler, PandaSystem::get_global_ptr()->get_compiler, std::string);
        REGISTER_STRING(get_build_date, PandaSystem::get_global_ptr()->get_build_date, std::string);
        REGISTER_STRING(get_git_commit, PandaSystem::get_global_ptr()->get_git_commit, std::string);
        REGISTER_STRING(get_platform, PandaSystem::get_global_ptr()->get_platform, std::string);

        REGISTER_FUNC(all_windows_closed, g_framework.all_windows_closed, bool);
        REGISTER_VOID_FUNC(clear_exit_flag, g_framework.clear_exit_flag);
        REGISTER_VOID_FUNC(clear_highlight, g_framework.clear_highlight);
        REGISTER_VOID_FUNC(close_all_windows, g_framework.close_all_windows);
        REGISTER_VOID_FUNC(close_framework, g_framework.close_framework);
        REGISTER_VOID_FUNC(open_framework, g_framework.open_framework);
        REGISTER_FUNC(open_window, g_framework.open_window, WindowFramework*);
        
        REGISTER("set_window_title", 1)
            g_framework.set_window_title(std::string(b_cstring(args, 0)));
        END_REGISTER

        REGISTER_VOID_FUNC(enable_default_keys, g_framework.enable_default_keys);
        REGISTER_VOID_FUNC(main_loop, g_framework.main_loop);

        REGISTER_FUNC(get_background_type, g_framework.get_background_type, int);
        REGISTER_FUNC(get_num_windows, g_framework.get_num_windows, int);
        
        REGISTER("load_prc_file_data", 2)
            load_prc_file_data(std::string(b_cstring(args, 0)), std::string(b_cstring(args, 1)));
        END_REGISTER
    
        
        //~ load_prc_file_data("", "load-display pandagl");
    }

    // Call by string name
    void* call_func(const char* name) {
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second(buf, argbuffer_argc(buf));
        } else {
            std::cerr << "No such function: " << name << "\n";
            return nullptr;
        }
    }
    
    ArgBuffer* create() {
        //You should probs never use this but like the option is here
        return argbuffer_create();
    }

    //Use the static buffer
    
    void buf_reset_destroy() {
        // TODO: implement or forward
        argbuffer_destroy(buf);
        buf = argbuffer_create();
    }
    
    void returns_reset_destroy() {
        argbuffer_destroy(returnBuf);
        returnBuf = argbuffer_create();
    }

    void push_cstring(const char* v) {
        argbuffer_push_cstring(buf, v);
    }

    void push_function(void* v) {
        argbuffer_push_function(buf, v);
    }

    void push_ptr(void* v) {
        argbuffer_push_ptr(buf, v);
    }

    void push_i8(int8_t v) {
        argbuffer_push_i8(buf, v);
    }

    void push_i16(int16_t v) {
        argbuffer_push_i16(buf, v);
    }

    void push_i32(int32_t v) {
        argbuffer_push_i32(buf, v);
    }

    void push_i64(int64_t v) {
        argbuffer_push_i64(buf, v);
    }

    void push_u8(uint8_t v) {
        argbuffer_push_u8(buf, v);
    }

    void push_u16(uint16_t v) {
        argbuffer_push_u16(buf, v);
    }

    void push_u32(uint32_t v) {
        argbuffer_push_u32(buf, v);
    }

    void push_u64(uint64_t v) {
        argbuffer_push_u64(buf, v);
    }

    void push_f32(float v) {
        argbuffer_push_f32(buf, v);
    }

    void push_f64(double v) {
        argbuffer_push_f64(buf, v);
    }

    void push_bool(bool v) {
        argbuffer_push_bool(buf, v);
    }

    void push_char(char v) {
        argbuffer_push_char(buf, v);
    }
    
    //---
    const char* get_cstring(int v) {
        return argbuffer_get_cstring(buf, v);
    }

    void* get_function(int v) {
        return argbuffer_get_function(buf, v);
    }

    void* get_ptr(int v) {
        return argbuffer_get_ptr(buf, v);
    }

    int8_t get_i8(int v) {
        return argbuffer_get_i8(buf, v);
    }

    int16_t get_i16(int v) {
        return argbuffer_get_i16(buf, v);
    }

    int32_t get_i32(int v) {
        return argbuffer_get_i32(buf, v);
    }

    int64_t get_i64(int v) {
        return argbuffer_get_i64(buf, v);
    }

    uint8_t get_u8(int v) {
        return argbuffer_get_u8(buf, v);
    }

    uint16_t get_u16(int v) {
        return argbuffer_get_u16(buf, v);
    }

    uint32_t get_u32(int v) {
        return argbuffer_get_u32(buf, v);
    }

    uint64_t get_u64(int v) {
        return argbuffer_get_u64(buf, v);
    }

    float get_f32(int v) {
        return argbuffer_get_f32(buf, v);
    }

    double get_f64(int v) {
        return argbuffer_get_f64(buf, v);
    }

    bool get_bool(int v) {
        return argbuffer_get_bool(buf, v);
    }

    char get_char(int v) {
        return argbuffer_get_char(buf, v);
    }

    std::string get_string(const char * c) {
        auto it = registry.find(c);
        if (it != registry.end()) {
            return it->second(buf, argbuffer_argc(buf));
        } else {
            std::cerr << "No such function: " << c << "\n";
            return nullptr;
        }
        return nullptr;
    }

    int argc() {
        return argbuffer_argc(buf);
    }

    void** as_void_array(ArgBuffer* buf) {
        //This should also never be called directly
        void** args = argbuffer_as_void_array(buf);
        return args;
    }

    const char* types() {
        return argbuffer_types(buf);
    }

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
