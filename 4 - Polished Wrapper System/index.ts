import { CString } from "bun:ffi";
import panda from "./panda";

async function main() {
    push_bool(true);
    push_f64(123.456);
    push_cstring("hi ffi");

    console.log(argc())
    console.log(types())

    reset_buffer();
    push_bool(true);
    push_cstring("hi ffi");
    push_f64(123.456);

    console.log(argc())
    console.log(types())
    
    reset_buffer();
    
    init_registry();
    
    push_cstring("")
    push_cstring("load-display pandagl")
    call_func("load_prc_file_data");
    reset_buffer();
    
    call_func("open_framework");
    push_cstring("this works but it's kinda dumb");
    console.log(types())
    console.log(get_cstring(0))
    call_func("set_window_title");
    reset_buffer();
    call_func("open_window");
    reset_buffer();
    


    
    call_func("main_loop");
    call_func("close_framework");
    
    
}
function push_cstring(str){
    panda.symbols.push_cstring(toCStringBuff(str));
}

function call_func(str){
    panda.symbols.call_func(toCStringBuff(str));
}

const get_cstring = panda.symbols.get_cstring;

const init_registry = panda.symbols.init_registry;
const push_bool = panda.symbols.push_bool;
const push_f64 = panda.symbols.push_f64;
const reset_buffer = panda.symbols.buf_reset_destroy;

const argc = panda.symbols.argc;
const types = panda.symbols.types;

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

function toCStringBuff(str){
    const strBuffer = new TextEncoder().encode(str + "\0"); 
    return strBuffer
}

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
