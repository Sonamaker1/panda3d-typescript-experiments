import { dlopen, FFIType } from "bun:ffi";
import { join } from "path";
const cwd = process.cwd();

const sdkPath = `${cwd}/modules/libpanda_wrapper.so`;

export const panda = dlopen(sdkPath, {
  "init_registry": {
    "args": [],
    "returns": FFIType.void
  },
  "call_func": {
    "args": [FFIType.cstring],
    "returns": FFIType.ptr
  },
  "create": {
    "args": [],
    "returns": FFIType.ptr
  },
  "buf_reset_destroy": {
    "args": [],
    "returns": FFIType.void
  },
  "push_cstring": {
    "args": [FFIType.cstring],
    "returns": FFIType.void
  },
  "push_function": {
    "args": [FFIType.function],
    "returns": FFIType.void
  },
  "push_ptr": {
    "args": [FFIType.ptr],
    "returns": FFIType.void
  },
  "push_i8": {
    "args": [FFIType.i8],
    "returns": FFIType.void
  },
  "push_i16": {
    "args": [FFIType.i16],
    "returns": FFIType.void
  },
  "push_i32": {
    "args": [FFIType.i32],
    "returns": FFIType.void
  },
  "push_i64": {
    "args": [FFIType.i64],
    "returns": FFIType.void
  },
  "push_u8": {
    "args": [FFIType.u8],
    "returns": FFIType.void
  },
  "push_u16": {
    "args": [FFIType.u16],
    "returns": FFIType.void
  },
  "push_u32": {
    "args": [FFIType.u32],
    "returns": FFIType.void
  },
  "push_u64": {
    "args": [FFIType.u64],
    "returns": FFIType.void
  },
  "push_f32": {
    "args": [FFIType.f32],
    "returns": FFIType.void
  },
  "push_f64": {
    "args": [FFIType.f64],
    "returns": FFIType.void
  },
  "push_bool": {
    "args": [FFIType.bool],
    "returns": FFIType.void
  },
  "push_char": {
    "args": [FFIType.char],
    "returns": FFIType.void
  },
  
  "get_cstring": {
    "args": [FFIType.i32],
    "returns": FFIType.cstring
  },
  "get_function": {
    "args": [FFIType.i32],
    "returns": FFIType.function
  },
  "get_ptr": {
    "args": [FFIType.i32],
    "returns": FFIType.ptr
  },
  "get_i8": {
    "args": [FFIType.i32],
    "returns": FFIType.i8
  },
  "get_i16": {
    "args": [FFIType.i32],
    "returns": FFIType.i16
  },
  "get_i32": {
    "args": [FFIType.i32],
    "returns": FFIType.i32
  },
  "get_i64": {
    "args": [FFIType.i32],
    "returns": FFIType.i64
  },
  "get_u8": {
    "args": [FFIType.i32],
    "returns": FFIType.u8
  },
  "get_u16": {
    "args": [FFIType.i32],
    "returns": FFIType.u16
  },
  "get_u32": {
    "args": [FFIType.i32],
    "returns": FFIType.u32
  },
  "get_u64": {
    "args": [FFIType.i32],
    "returns": FFIType.u64
  },
  "get_f32": {
    "args": [FFIType.i32],
    "returns": FFIType.f32
  },
  "get_f64": {
    "args": [FFIType.i32],
    "returns": FFIType.f64
  },
  "get_bool": {
    "args": [FFIType.i32],
    "returns": FFIType.bool
  },
  "get_char": {
    "args": [FFIType.i32],
    "returns": FFIType.char
  },
  
  "argc": {
    "args": [],
    "returns": FFIType.i32
  },
  "as_void_array": {
    "args": [],
    "returns": FFIType.void
  },
  "types": {
    "args": [],
    "returns": FFIType.cstring
  }
});
export default panda;




//~ import wrapperLib from "./modules/libpanda_wrapper.so" with { type: "file"};
//~ import { file } from "bun"

//~ import { dlopen, FFIType } from "bun:ffi";
//~ import { join } from "path";

//~ const libPath = join(import.meta.dir, "libpanda_wrapper.so");

//~ export const panda = dlopen(libPath, {
  //~ panda_init: { args: [FFIType.i32, FFIType.i32], returns: FFIType.void },
  //~ panda_load_model: { args: [FFIType.cstring], returns: FFIType.void },
  //~ panda_rotate_model: { args: [FFIType.f32], returns: FFIType.void },
  //~ panda_run: { args: [], returns: FFIType.void },
  //~ panda_shutdown: { args: [], returns: FFIType.void },
//~ });

//~ export default panda;


//~ import { dlopen, FFIType, suffix } from "bun:ffi";
//~ const { i32, cstring, ptr, f32} = FFIType;
//~ import { join } from "path";

//~ const libPath = join(import.meta.dir, "libpanda_wrapper.so");

//~ const panda = dlopen(libPath, {
    //~ panda_init: { args: [i32, i32], result: ptr },
    //~ panda_load_model: { args: ['cstring'], result: 'ptr'},
    //~ panda_rotate_model: { args: ['f32', 'f32', 'f32'], result: "void" },
    //~ panda_run: { args: [], result: "void" },
    //~ panda_shutdown: { args: [], result: "void" },
//~ });

//~ export default panda;

//~ import { loadLibrary, type FFISymbols } from "./ffi";

//~ const lib = await loadLibrary("./libpanda_wrapper.so"); // or .dll on Windows

//~ const panda = {
    //~ panda_init: { params: ["i32", "i32"], result: "pointer" },
    //~ panda_load_model: { params: ["cstring"], result: "pointer" },
    //~ panda_rotate_model: { params: ["f32", "f32", "f32"], result: "void" },
    //~ panda_run: { params: [], result: "void" },
    //~ panda_shutdown: { params: [], result: "void" },
//~ };

//~ export default panda;
