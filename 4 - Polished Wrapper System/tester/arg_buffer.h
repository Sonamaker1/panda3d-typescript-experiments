#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

// Forward decl
typedef struct ArgBuffer ArgBuffer;

// Creation / destruction
ArgBuffer* argbuffer_create();
void argbuffer_destroy(ArgBuffer* buf);

// Push functions (auto-generated)
void argbuffer_push_buffer(ArgBuffer* buf, char* v);
void argbuffer_push_cstring(ArgBuffer* buf, const char* v);
void argbuffer_push_function(ArgBuffer* buf, void* v);
void argbuffer_push_ptr(ArgBuffer* buf, void* v);
void argbuffer_push_i8(ArgBuffer* buf, int8_t v);
void argbuffer_push_i16(ArgBuffer* buf, int16_t v);
void argbuffer_push_i32(ArgBuffer* buf, int32_t v);
void argbuffer_push_i64(ArgBuffer* buf, int64_t v);
void argbuffer_push_i64_fast(ArgBuffer* buf, int64_t v);
void argbuffer_push_u8(ArgBuffer* buf, uint8_t v);
void argbuffer_push_u16(ArgBuffer* buf, uint16_t v);
void argbuffer_push_u32(ArgBuffer* buf, uint32_t v);
void argbuffer_push_u64(ArgBuffer* buf, uint64_t v);
void argbuffer_push_u64_fast(ArgBuffer* buf, uint64_t v);
void argbuffer_push_f32(ArgBuffer* buf, float v);
void argbuffer_push_f64(ArgBuffer* buf, double v);
void argbuffer_push_bool(ArgBuffer* buf, bool v);
void argbuffer_push_char(ArgBuffer* buf, char v);
void argbuffer_push_napi_env(ArgBuffer* buf, void* v);
void argbuffer_push_napi_value(ArgBuffer* buf, void* v);

// Accessors
int argbuffer_argc(ArgBuffer* buf);
void** argbuffer_as_void_array(ArgBuffer* buf);
const char* argbuffer_types(ArgBuffer* buf);

#ifdef __cplusplus
}
#endif
