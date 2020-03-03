#include "stdio.h"

#define page_size_shift 5
#define page_size 32
#define phym_size (1 << 12)
#define page_num (phym_size / page_size)

static int phym[page_num][page_size];

int main(void) {
    for (int i = 0; i < page_num; ++i) {
        for (int j = 0; j < page_size; ++j) {
            scanf("%x", &phym[i][j]);
        }
    }

    /*for (int i = 0; i < page_num; ++i) {*/
        /*for (int j = 0; j < page_size; ++j) {*/
            /*printf("%02x ", phym[i][j]);*/
        /*}*/
        /*putchar('\n');*/
    /*}*/

    int vaddr = 0x748b;
    printf("virtual addr:0x%04x\n", vaddr);

    int* pdt = phym[0x11];
    int pdt_index = vaddr >> 10;
    int pt_index = (vaddr >> 5) & 0x1f;
    int pg_offset = vaddr & 0x1f;

    int pde = pdt[pdt_index]; 
    int pde_valid = pde >> 7;
    int ptn = pde & 0x7f;
    printf("pdt index:0x%02x pde:(valid: %d, ptn 0x%02x)\n", pdt_index, pde_valid, ptn);

    if (pde_valid) {
        int* pt = phym[ptn]; 
        int pte = pt[pt_index];
        int pte_valid = pte >> 7;
        int ppn = pte & 0x7f;
        printf("pt index:0x%02x pte:(valid: %d, ppn 0x%02x)\n", pt_index, pte_valid, ppn);

        if (pte_valid) {
            int phy_addr = (ppn << page_size_shift) + pg_offset;
            int by = phym[ppn][pg_offset];
            printf("physical addr:0x%02x content:0x%02x\n", phy_addr, by);
        }
    }
}
