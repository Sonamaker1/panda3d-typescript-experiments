import { CString } from "bun:ffi";
import panda from "./panda";

async function main() {
    //~ push_bool(true);
    //~ push_f64(123.456);
    //~ push_cstring("hi ffi");

    //~ console.log(argc())
    //~ console.log(types())

    //~ reset_buffer();
    //~ push_bool(true);
    //~ push_cstring("hi ffi");
    //~ push_f64(123.456);

    //~ console.log(argc())
    //~ console.log(types())
    
    reset_buffer();
    
    init_registry();

    load_prc_file_data("","load-display pandagl");

    open_framework();
    set_window_title("this works but it's kinda dumb");
    open_window();

    main_loop();
    close_framework();
    
    
}


function toCStringBuff(str){
    const strBuffer = new TextEncoder().encode(str + "\0"); 
    return strBuffer
}

function push_cstring(str){
    panda.symbols.push_cstring(toCStringBuff(str));
}

// Type hint enum
type TypeHint = "bool" | "i32" | "f64" | "cstring" | "ptr";
function defineFunc(name: string, types?: TypeHint[]) {
    return (...args: any[]) => {
        if (types && args.length !== types.length) {
            throw new Error(`${name} expects ${types.length} arguments, got ${args.length}`);
        }

        for (let i = 0; i < args.length; i++) {
            const type = types ? types[i] : undefined;
            const arg = args[i];

            switch (type ?? typeof arg) {
                case "bool":
                    panda.symbols.push_bool(arg);
                    break;
                case "i32":
                    panda.symbols.push_i32(arg);
                    break;
                case "f64":
                    panda.symbols.push_f64(arg);
                    break;
                case "cstring":
                    push_cstring(arg);
                    break;
                case "ptr":
                    panda.symbols.push_ptr(arg);
                    break;
                default:
                    // Auto-detect if type hint wasn't provided
                    if (typeof arg === "boolean") push_bool(arg);
                    else if (typeof arg === "number") {
                        Number.isInteger(arg) ? push_i32(arg) : push_f64(arg);
                    }
                    else if (typeof arg === "string") push_cstring(arg);
                    else throw new Error(`Unsupported argument type for ${name}: ${typeof arg}`);
            }
        }

        var result = panda.symbols.call_func(toCStringBuff(name));
        reset_buffer();
        return result;
    };
}
const init_registry = panda.symbols.init_registry;
const reset_buffer = panda.symbols.buf_reset_destroy;

const set_window_title = defineFunc("set_window_title", ["cstring"]);
const load_prc_file_data = defineFunc("load_prc_file_data", ["cstring", "cstring"]);
const open_window = defineFunc("open_window", []);
const main_loop = defineFunc("main_loop", []);
const open_framework = defineFunc("open_framework", []);
const close_framework = defineFunc("close_framework", []);



//~ // Your FFI entry
//~ declare function call_function(name: string, ...args: any[]): any;

//~ // Helper to define passthroughs
//~ function defineFunc(name: string): (...args: any[]) => any {
  //~ return (...args: any[]) => call_function(name, ...args);
//~ }

//~ // Usage: create a wrapper for main_loop
//~ const open_framework = defineFunc("open_framework");
//~ const main_loop = defineFunc("main_loop");




//~ function call_func(str){
    //~ panda.symbols.call_func(toCStringBuff(str));
//~ }

//~ function call_func_r(str){
    //~ panda.symbols.call_func(toCStringBuff(str));
    //~ reset_buffer();
//~ }

//~ const get_cstring = panda.symbols.get_cstring;
//~ const push_bool = panda.symbols.push_bool;
//~ const push_f64 = panda.symbols.push_f64;
//~ const reset_buffer = panda.symbols.buf_reset_destroy;

//~ const argc = panda.symbols.argc;
//~ const types = panda.symbols.types;

//~ async function main() {
    //~ console.log("Starting Panda3D…");
    
    //~ load_prc_file_data("", "load-display pandagl");
    //~ open_framework();
    //~ set_window_title("Panda3D from TypeScript!");
    //~ const window = open_window();
    //~ main_loop();
    //~ close_framework();
//~ }


//~ const open_framework = panda.symbols.p_open_framework;
//~ const open_window = panda.symbols.p_open_window;
//~ const main_loop = panda.symbols.p_main_loop;
//~ const close_framework = panda.symbols.p_close_framework;


//~ function load_prc_file_data(name, data){
    //~ panda.symbols.p_load_prc_file_data(toCStringBuff(name),toCStringBuff(data));
//~ }
//~ function set_window_title(title){
    //~ panda.symbols.p_set_window_title(toCStringBuff(title));
//~ }

main();



    
    //~ framework.set_window_title("Panda3D from TypeScript!");
    //~ window = framework.open_window();

    //~ panda.symbols.panda_init(800, 600);
    
    //~ // Encode a JS string into a null-terminated UTF-8 buffer
    //~ const modelName = "/home/w/.wine/drive_c/users/w/AppData/Local/Corporate Clash/resources/extracted/toontown_central.bam";
    //~ const modelBuffer = new TextEncoder().encode(modelName + "\0"); // add null terminator

    //~ // Pass it to FFI as a pointer
    //~ const modelPtr = panda.symbols.panda_load_model(modelBuffer);

    // Rotate every frame in a separate loop
    //~ // this doesn't work
    //~ setInterval(() => {
        //~ panda.symbols.panda_rotate_model(modelPtr,1.0, 1.0, 0.0);
    //~ }, 16); // 60 fps

    //~ panda.symbols.panda_run();
    //~ panda.symbols.panda_shutdown();
