#include "arg_buffer.h"
#include <stdio.h>

int main() {
    printf("Hello, C world!\n");

    ArgBuffer* buf = argbuffer_create();
    argbuffer_push_bool(buf, true);
    argbuffer_push_cstring(buf, "hi ffi");
    argbuffer_push_f64(buf, 123.456);
    
    void** args = argbuffer_as_void_array(buf);
    int argc = argbuffer_argc(buf);

    printf("Type sig: %s\n", argbuffer_types(buf));
    // -> "bool_f64_cstring"
    //~ void* result = call_func("SomeFunction", (void*)g_framework, args, argc);

    argbuffer_destroy(buf);

    return 0; // Indicates successful execution
}


    //~ // Example
    //~ ArgBuffer* buf = argbuffer_create();
    //~ argbuffer_push_bool(buf, true);
    //~ argbuffer_push_f64(buf, 42.123);
    //~ argbuffer_push_cstring(buf, "hello ffi");

    //~ // Get raw args
    //~ void** args = argbuffer_as_void_array(buf);
    //~ int argc = argbuffer_argc(buf);

    //~ // Call
    //~ void* result = call_func("someFunction", (void*)g_framework, args, argc);

    //~ // Inspect type signature string
    //~ printf("Types: %s\n", argbuffer_types(buf)); // -> "bool_f64_cstring"

    //~ argbuffer_destroy(buf);
