#include "arg_buffer.h"
#include <string>
#include <vector>

struct ArgEntry {
    enum ArgType {
        ARG_BUFFER,
        ARG_CSTRING,
        ARG_FUNCTION,
        ARG_PTR,
        ARG_I8,
        ARG_I16,
        ARG_I32,
        ARG_I64,
        ARG_I64_FAST,
        ARG_U8,
        ARG_U16,
        ARG_U32,
        ARG_U64,
        ARG_U64_FAST,
        ARG_F32,
        ARG_F64,
        ARG_BOOL,
        ARG_CHAR,
        ARG_NAPI_ENV,
        ARG_NAPI_VALUE
    } type;

    union {
        char* buffer;
        const char* cstring;
        void* function;
        void* ptr;
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        float f32;
        double f64;
        bool b;
        char c;
        void* napi_env;
        void* napi_value;
    } value;
};

struct ArgBuffer {
    std::vector<ArgEntry> args;
    std::string type_sig;
};

static void push_type(ArgBuffer* buf, const char* tag) {
    if (!buf->type_sig.empty()) buf->type_sig += "_";
    buf->type_sig += tag;
}

extern "C" {

ArgBuffer* argbuffer_create() {
    return new ArgBuffer();
}

void argbuffer_destroy(ArgBuffer* buf) {
    delete buf;
}

// ---- AUTO-GENERATED PUSH FUNCTIONS ----
#define DEF_PUSH(NAME, FIELD, TAG, TYPE, VAL)          \
    void argbuffer_push_##NAME(ArgBuffer* buf, TYPE v) { \
        ArgEntry e;                                    \
        e.type = ArgEntry::ARG_##FIELD;                \
        e.value.VAL = v;                               \
        buf->args.push_back(e);                        \
        push_type(buf, TAG);                           \
    }

DEF_PUSH(buffer, BUFFER, "buffer", char*, buffer)
DEF_PUSH(cstring, CSTRING, "cstring", const char*, cstring)
DEF_PUSH(function, FUNCTION, "function", void*, function)
DEF_PUSH(ptr, PTR, "ptr", void*, ptr)
DEF_PUSH(i8, I8, "i8", int8_t, i8)
DEF_PUSH(i16, I16, "i16", int16_t, i16)
DEF_PUSH(i32, I32, "i32", int32_t, i32)
DEF_PUSH(i64, I64, "i64", int64_t, i64)
DEF_PUSH(i64_fast, I64_FAST, "i64_fast", int64_t, i64)
DEF_PUSH(u8, U8, "u8", uint8_t, u8)
DEF_PUSH(u16, U16, "u16", uint16_t, u16)
DEF_PUSH(u32, U32, "u32", uint32_t, u32)
DEF_PUSH(u64, U64, "u64", uint64_t, u64)
DEF_PUSH(u64_fast, U64_FAST, "u64_fast", uint64_t, u64)
DEF_PUSH(f32, F32, "f32", float, f32)
DEF_PUSH(f64, F64, "f64", double, f64)
DEF_PUSH(bool, BOOL, "bool", bool, b)
DEF_PUSH(char, CHAR, "char", char, c)
DEF_PUSH(napi_env, NAPI_ENV, "napi_env", void*, napi_env)
DEF_PUSH(napi_value, NAPI_VALUE, "napi_value", void*, napi_value)

#undef DEF_PUSH
// --------------------------------------

int argbuffer_argc(ArgBuffer* buf) {
    return (int)buf->args.size();
}

void** argbuffer_as_void_array(ArgBuffer* buf) {
    static std::vector<void*> scratch;
    scratch.clear();
    scratch.reserve(buf->args.size());
    for (auto& e : buf->args) {
        scratch.push_back((void*)&e.value);
    }
    return scratch.data();
}

const char* argbuffer_types(ArgBuffer* buf) {
    return buf->type_sig.c_str();
}

} // extern "C"
