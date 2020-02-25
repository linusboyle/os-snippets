#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

using addr = uint64_t;

addr *stack_bottom;
 
void print_stacktrace() {
    int size = 100;
    void * array[100];
    int stack_num = backtrace(array, size);
    char ** stacktrace = backtrace_symbols(array, stack_num);
    int num = stack_num - 3;
    printf("A total of %d stacks\n", num);
     
    register addr *current_ebp __asm__ ("%rbp");
    addr *ebp = current_ebp;
    int k = 0;
    while (ebp != stack_bottom) {
        printf("stack %d: ", num-k);
        printf("%s\n", stacktrace[++k]);
        addr *upper_ebp = (addr*) *ebp; 
        int len = upper_ebp - ebp;
        printf("(0x%0lx) ", reinterpret_cast<addr>(ebp));
        for (int i = 0; i < len; i++) 
            printf("0x%0lx ", *(ebp+i));
        printf("\n");
        ebp = upper_ebp;
    }
    free(stacktrace);
}

void fun1() {
    printf("stack trace begin:\n");
    print_stacktrace();
}

void fun2(int a) {
    fun1();
}

void fun3(int a, int b) {
    fun2(3);
}

int main() {
    register addr *main_ebp __asm__ ("%rbp");
    stack_bottom = main_ebp; 
    fun3(1, 2);
}
