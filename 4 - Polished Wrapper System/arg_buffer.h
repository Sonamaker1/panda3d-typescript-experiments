#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

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
//~ void argbuffer_push_napi_env(ArgBuffer* buf, void* v);
//~ void argbuffer_push_napi_value(ArgBuffer* buf, void* v);

// Get functions (mirrors push)
char* argbuffer_get_buffer(ArgBuffer* buf, int idx);
const char* argbuffer_get_cstring(ArgBuffer* buf, int idx);
void* argbuffer_get_function(ArgBuffer* buf, int idx);
void* argbuffer_get_ptr(ArgBuffer* buf, int idx);
int8_t argbuffer_get_i8(ArgBuffer* buf, int idx);
int16_t argbuffer_get_i16(ArgBuffer* buf, int idx);
int32_t argbuffer_get_i32(ArgBuffer* buf, int idx);
int64_t argbuffer_get_i64(ArgBuffer* buf, int idx);
int64_t argbuffer_get_i64_fast(ArgBuffer* buf, int idx);
uint8_t argbuffer_get_u8(ArgBuffer* buf, int idx);
uint16_t argbuffer_get_u16(ArgBuffer* buf, int idx);
uint32_t argbuffer_get_u32(ArgBuffer* buf, int idx);
uint64_t argbuffer_get_u64(ArgBuffer* buf, int idx);
uint64_t argbuffer_get_u64_fast(ArgBuffer* buf, int idx);
float argbuffer_get_f32(ArgBuffer* buf, int idx);
double argbuffer_get_f64(ArgBuffer* buf, int idx);
bool argbuffer_get_bool(ArgBuffer* buf, int idx);
char argbuffer_get_char(ArgBuffer* buf, int idx);
//~ void* argbuffer_get_napi_env(ArgBuffer* buf, int idx);
//~ void* argbuffer_get_napi_value(ArgBuffer* buf, int idx);

// Accessors
int argbuffer_argc(ArgBuffer* buf);
void** argbuffer_as_void_array(ArgBuffer* buf);
const char* argbuffer_types(ArgBuffer* buf);

#ifdef __cplusplus
}
#endif
