#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


uint32_t asm_crc32c(const char *bytes, size_t len){
    if (!len) {
        len = strlen(bytes);
    }

    int64_t result;
    asm("mov %[result], 0xFFFFFFFF\n"

        "mov r9, %[len]\n"
        "and r9b, 0b11111000\n"

        "mov rbx, %[data_pointer]\n"
        "add rbx, r9\n"
        "not_done_8:\n"
        "cmp %[data_pointer], rbx\n"
        "je done_8\n"

        "crc32  %[result], qword ptr [%[data_pointer]]\n"

        "add %[data_pointer], 8\n"
        "jne not_done_8\n"
        "done_8:\n"

        "sub %[len], r9\n"
        "add rbx, %[len]\n"
        "not_done_1:\n"
        "cmp %[data_pointer], rbx\n"
        "je end_of_data\n"
        "crc32 %[result], byte ptr [%[data_pointer]]\n"
        "inc %[data_pointer]\n"
        "jmp not_done_1\n"
        "end_of_data:\n"
            :[result]"+r"(result)
            :[data_pointer]"r"((uint32_t*)bytes), [len]"r"(len)
            :"rbx", "r9"
            );


    return result^0xFFFFFFFF; // Compiler optimizes this to "not eax"
}

void test() {
    // Test cases generated with https://crccalc.com/, Result in the CRC-32C algorithm
    //assert(asm_crc32c("AsdFJklo",0) ==0x5899CC2E) ;
    assert(asm_crc32c("A",0) ==         0xE16DCDEE) ;
    assert(asm_crc32c("",0) ==          0x0) ;
    assert(asm_crc32c("",1) == 	      0x527D5351) ; // CRC32c of null
    assert(asm_crc32c(" ",1) ==         0x72C0DD8F) ;
    assert(asm_crc32c("AsdFJklof",0)==  0x99878FEE) ;
    assert(asm_crc32c("Sixbyt",0)== 	  0xF1119B6C) ;
    assert(asm_crc32c("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",0)
                                                ==0x95DC2E4B) ;
}


int main() {
    test();
    printf("All test cases OK\n");
}