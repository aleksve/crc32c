#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

uint32_t asm_crc32c(unsigned const char *bytes, size_t len);

int main() {
    char * bytes = "AsdFJklof";
    //char * bytes = "fdsA";
    size_t len = strlen(bytes);

    printf("Bytes: %s, len: %ld, result: %x\n", bytes, len, asm_crc32c(bytes, len));
    return 0;
}
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
uint32_t asm_crc32c(unsigned const char *bytes, size_t len){
    int  result;

    asm("mov %[result], 0xFFFFFFFF\n"

        "mov r9, %[len]\n"
        "shr r9, 2\n"
        "shl r9, 2\n"

        "mov rbx, %[data_pointer]\n"
        "add rbx, r9\n"
        "not_done_4:\n"

        "crc32  %[result], dword ptr [%[data_pointer]]\n"

        "add %[data_pointer], 4\n"
        "cmp %[data_pointer], rbx\n"
        "jne not_done_4\n"

        "sub %[len], r9\n"
        "add rbx, %[len]\n"
        "cmp %[data_pointer], rbx\n"
        "je end_of_data\n"
        "crc32 %[result], byte ptr [%[data_pointer]]\n"
        "inc %[data_pointer]\n"
        "end_of_data:"
            :[result]"+r"(result)
            :[data_pointer]"r"((uint32_t*)bytes), [len]"r"(len)
            :"rbx", "r9"
            );


    return result^0xFFFFFFFF; // Compiler optimizes this to "not eax"
}


