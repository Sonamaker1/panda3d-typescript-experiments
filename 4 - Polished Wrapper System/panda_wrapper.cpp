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

static WindowFramework *window = nullptr;
static NodePath render;
static NodePath *model  = nullptr;
static ArgBuffer* buf =  argbuffer_create();

static PandaFramework g_framework;  // global instance
using Dispatcher = std::function<void*(ArgBuffer*, const char*)>;
std::unordered_map<const char*, Dispatcher> registry;

extern "C" {
    
    // Define your enum
    typedef enum {
        ALL_WINDOWS_CLOSED,
        CLEAR_EXIT_FLAG,
        CLEAR_HIGHLIGHT,
        //~ CLEAR_TEXT,
        CLOSE_ALL_WINDOWS,
        CLOSE_FRAMEWORK,
        //~ DO_ENABLE_DEFAULT_KEYS,
        ENABLE_DEFAULT_KEYS,
        funct_UNKNOWN,
        GET_BACKGROUND_TYPE,
        GET_DATA_ROOT,
        GET_DEFAULT_PIPE,
        GET_EVENT_HANDLER,
        GET_GRAPHICS_ENGINE,
        GET_HIGHLIGHT,
        GET_LIGHTING,
        GET_MODELS,
        GET_NUM_WINDOWS,
        GET_PERPIXEL,
        GET_RECORDER,
        GET_TASK_MGR,
        GET_TEXTURE,
        GET_TWO_SIDED,
        GET_WIREFRAME,
        HAS_HIGHLIGHT,
        MAIN_LOOP,
        //~ MAKE_DEFAULT_PIPE,
        OPEN_FRAMEWORK,
        OPEN_WINDOW,
        PANDAFRAMEWORK,
        //pandaFramework stuff
        RESET_FRAME_RATE,
        SET_EXIT_FLAG,

    } funct_t;
    
    static funct_t list[] = {
        ALL_WINDOWS_CLOSED,
        CLEAR_EXIT_FLAG,
        CLEAR_HIGHLIGHT,
        //~ CLEAR_TEXT,
        CLOSE_ALL_WINDOWS,
        CLOSE_FRAMEWORK,
        //~ DO_ENABLE_DEFAULT_KEYS,
        ENABLE_DEFAULT_KEYS,
        funct_UNKNOWN,
        GET_BACKGROUND_TYPE,
        GET_DATA_ROOT,
        GET_DEFAULT_PIPE,
        GET_EVENT_HANDLER,
        GET_GRAPHICS_ENGINE,
        GET_HIGHLIGHT,
        GET_LIGHTING,
        GET_MODELS,
        GET_NUM_WINDOWS,
        GET_PERPIXEL,
        GET_RECORDER,
        GET_TASK_MGR,
        GET_TEXTURE,
        GET_TWO_SIDED,
        GET_WIREFRAME,
        HAS_HIGHLIGHT,
        MAIN_LOOP,
        //~ MAKE_DEFAULT_PIPE,
        OPEN_FRAMEWORK,
        OPEN_WINDOW,
        PANDAFRAMEWORK,
        //pandaFramework stuff
        RESET_FRAME_RATE,
        SET_EXIT_FLAG
    };

    // Create the array 
    static const char* funct_map[] = {
        "all_windows_closed",
        "clear_exit_flag",
        "clear_highlight",
        //~ "clear_text",
        "close_all_windows",
        "close_framework",
        //~ "do_enable_default_keys",
        "enable_default_keys",
        "funct_unknown",
        "get_background_type",
        "get_data_root",
        "get_default_pipe",
        "get_event_handler",
        "get_graphics_engine",
        "get_highlight",
        "get_lighting",
        "get_models",
        "get_num_windows",
        "get_perpixel",
        "get_recorder",
        "get_task_mgr",
        "get_texture",
        "get_two_sided",
        "get_wireframe",
        "has_highlight",
        "main_loop",
        //~ "make_default_pipe",
        "open_framework",
        "open_window",
        "PandaFramework",
        "reset_frame_rate",
        "set_exit_flag",
    };
    
    static int lookupsize = sizeof(funct_map) / sizeof(funct_map[0]);

    int findStringInSortedArray(const char *arr[], const char *target) {
        int low = 0;
        int high = lookupsize - 1;
       
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int cmp_result = strcmp(target, arr[mid]);
            if (cmp_result == 0) {
                return mid; // String found at index mid
            } else if (cmp_result < 0) {
                high = mid - 1; // Search in the left half
            } else {
                low = mid + 1;  // Search in the right half
            }
        }
        return -1; // String not found
    }

    // Function to convert a string to an enum using bsearch()
    funct_t string_to_funct(const char* funct_name) {
        // Perform binary search on the sorted table
        int indx = findStringInSortedArray(funct_map, funct_name);

        if (indx > -1) {
            return list[indx];
        } else {
            std::cerr << "found nothing for: " << funct_name << "..." << indx << "\n";
            return funct_UNKNOWN;
        }
    }
    // Register functions (note, registry functions CANNOT return be used to return a primative, else memory issues)
    void init_registry() {
        
        registry["nothing"] = [](ArgBuffer*, const char* name) -> void* {
            switch (string_to_funct(name)) {
                case ALL_WINDOWS_CLOSED: g_framework.all_windows_closed(); return nullptr; break;
                case CLEAR_EXIT_FLAG: g_framework.clear_exit_flag(); return nullptr; break;
                case CLEAR_HIGHLIGHT: g_framework.clear_highlight(); return nullptr; break;
                //~ case CLEAR_TEXT: g_framework.clear_text(); return nullptr; break;
                case CLOSE_ALL_WINDOWS: g_framework.close_all_windows(); return nullptr; break;
                case CLOSE_FRAMEWORK: g_framework.close_framework(); return nullptr; break;
                //~ case DO_ENABLE_DEFAULT_KEYS: g_framework.do_enable_default_keys(); return nullptr; break;
                case ENABLE_DEFAULT_KEYS: g_framework.enable_default_keys(); return nullptr; break;
                case GET_BACKGROUND_TYPE: g_framework.get_background_type(); return nullptr; break;
                case GET_DATA_ROOT: g_framework.get_data_root(); return nullptr; break;
                case GET_DEFAULT_PIPE: g_framework.get_default_pipe(); return nullptr; break;
                case GET_EVENT_HANDLER: g_framework.get_event_handler(); return nullptr; break;
                case GET_GRAPHICS_ENGINE: g_framework.get_graphics_engine(); return nullptr; break;
                case GET_HIGHLIGHT: g_framework.get_highlight(); return nullptr; break;
                case GET_LIGHTING: g_framework.get_lighting(); return nullptr; break;
                case GET_MODELS: g_framework.get_models(); return nullptr; break;
                case GET_NUM_WINDOWS: g_framework.get_num_windows(); return nullptr; break;
                case GET_PERPIXEL: g_framework.get_perpixel(); return nullptr; break;
                case GET_RECORDER: g_framework.get_recorder(); return nullptr; break;
                case GET_TASK_MGR: g_framework.get_task_mgr(); return nullptr; break;
                case GET_TEXTURE: g_framework.get_texture(); return nullptr; break;
                case GET_TWO_SIDED: g_framework.get_two_sided(); return nullptr; break;
                case GET_WIREFRAME: g_framework.get_wireframe(); return nullptr; break;
                case HAS_HIGHLIGHT: g_framework.has_highlight(); return nullptr; break;
                case MAIN_LOOP: g_framework.main_loop(); return nullptr; break;
                //~ case MAKE_DEFAULT_PIPE: g_framework.make_default_pipe(); return nullptr; break;
                case OPEN_FRAMEWORK: g_framework.open_framework(); return nullptr; break;
                case OPEN_WINDOW: g_framework.open_window(); return nullptr; break;
                //~ case PANDAFRAMEWORK: PandaFramework(); return nullptr; break;
                case RESET_FRAME_RATE: g_framework.reset_frame_rate(); return nullptr; break;
                case SET_EXIT_FLAG: g_framework.set_exit_flag(); return nullptr; break;
                default:
                    //if no case matches
                    return nullptr;
                    break; // optional, but recommended for consistency
            }
            //This should never fire ... I HOPE it never does at least
            return nullptr;
        };
        registry["bool"] = [](ArgBuffer*, const char* name) -> void* {
            return nullptr; // returning pointer directly, caller should not free
        };
        registry["cstring_cstring"] = [](ArgBuffer*, const char* name) -> void* {
            return nullptr; // void return
        };
        
        load_prc_file_data("", "load-display pandagl");
    }

    // Call by string name, handles some overloads
    void* call_func(const char* name) {
        auto it = registry.find(argbuffer_types(buf));
        if (it != registry.end()) {
            return it->second(buf, name);
        } else if(strlen(argbuffer_types(buf)) == 0) {
            auto indx = registry.find("nothing");
            if(indx != registry.end()){
                return indx->second(buf, name);
            }
            else {
                std::cerr << "(nothing) no such function: " << name << "\n";
                std::cerr << "Buffer Types: " << strlen(argbuffer_types(buf)) << "\n";
                return nullptr;
            }
        } else {
            std::cerr << "No such function: " << name << "\n";
            std::cerr << "Buffer Types: " << strlen(argbuffer_types(buf)) << "\n";
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
