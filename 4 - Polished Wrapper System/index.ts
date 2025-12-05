import { CString } from "bun:ffi";
import panda from "./panda";

async function main() {
    
    init_registry();

    reset_buffer();

    load_prc_file_data("","load-display pandagl");

    open_framework();
    set_window_title("this works and it's getting better");
    //panda.symbols.push_cstring(get_version_string());

    //call_panda("set_window_title");
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

function call_panda(str){
    panda.symbols.call_func(toCStringBuff(str));
}
const init_registry = panda.symbols.init_registry;
const reset_buffer = panda.symbols.buf_reset_destroy;

const set_window_title = defineFunc("set_window_title", ["cstring"]);
const load_prc_file_data = defineFunc("load_prc_file_data", ["cstring", "cstring"]);
const open_window = defineFunc("open_window", []);
const main_loop = defineFunc("main_loop", []);
const open_framework = defineFunc("open_framework", []);
const close_framework = defineFunc("close_framework", []);

const get_version_string = defineFunc("get_version_string", []);
const get_distributor = defineFunc("get_distributor", []);
const get_compiler = defineFunc("get_compiler", []);
const get_build_date = defineFunc("get_build_date", []);
const get_platform = defineFunc("get_platform", []);



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

main();
