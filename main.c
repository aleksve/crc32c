#include <stdio.h>
#include <stdint.h>
#include <smmintrin.h>
#include <string.h>

uint32_t crc32(unsigned char*, size_t);

int main() {
    char * bytes = "Asdf";
    //char * bytes = "fdsA";
    size_t len = strlen(bytes);

    printf("Bytes: %s, len: %ld, result: %x\n", bytes, len, crc32(bytes, len));
    return 0;
}
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
uint32_t crc32(unsigned char *bytes, size_t len){
    uint32_t result = 0xFFFFFFFF;
    for (int x = 0; x < len; x+=4)
    {
        asm(
                "crc32 %[result], %[data_pointer]\n"
                :[result]"+r"(result)
                :[data_pointer]"r"(*(uint32_t*)bytes+x)
                );
        //result = _mm_crc32_u32(result, *(uint32_t*)bytes);
    }
    return result ^ 0xFFFFFFFF;

    //    asm("mov \n"
//        "inc rax\n"
//        "mov %[result], rax"
//
//        :[result]"=r"(result)
//        :[in]"r"(bytes), [len]"r"(len)
//    : "rax"
//                );

}


