#include <stdio.h>

int main() {
    // This how we combine things into 1 byte
    // I'm including this here because bitwise is hazy for me
    unsigned int i = 1;
    unsigned int j = 5;
    unsigned int a = (i << 4) | (j & 0xf);
    printf("i=%d, j=%d\n", (a >> 4), (a & 0xf));
    return 0;
}