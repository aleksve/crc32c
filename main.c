#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

uint32_t crc32(unsigned const char*, size_t);

int main() {
    char * bytes = "AsdFJklo";
    //char * bytes = "fdsA";
    size_t len = strlen(bytes);

    printf("Bytes: %s, len: %ld, result: %x\n", bytes, len, crc32(bytes, len));
    return 0;
}
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
uint32_t crc32(unsigned const char *bytes, size_t len){
    int  result;
    // Only sequences of length divisible by four is supported, because it is interpreted as a sequence of 32-bit workds's, and a special case is not yet written
    assert((len/4)*4 == len);
    asm("mov %[result], 0xFFFFFFFF\n"
        "mov rbx, %[data_pointer]\n"
        "add rbx, %[len]\n"
        "not_done:\n"

        "crc32  %[result], dword ptr [%[data_pointer]]\n"

        "add %[data_pointer], 4\n"
        "cmp %[data_pointer], rbx\n"
        "jne not_done\n"
            :[result]"+r"(result)
            :[data_pointer]"r"((uint32_t*)bytes), [len]"r"(len)
            :"rbx"
            );


    return result^0xFFFFFFFF; // Compiler optimizes this to "not eax"

    //    asm("mov \n"
//        "inc rax\n"
//        "mov %[result], rax"
//
//        :[result]"=r"(result)
//        :[in]"r"(bytes), [len]"r"(len)
//    : "rax"
//                );

}


