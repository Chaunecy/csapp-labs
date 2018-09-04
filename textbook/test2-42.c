#include<stdio.h>

int div16(int x) {
    int bias = (x >> 31) & 1;
    return (x >> 4) + bias;
}
int div16_another(int x) {
    int bias = (x >> 31) & 0xF;
    return (x + bias) >> 4;
}
void print(int n) {
     printf("my ans: %d, another ans: %d, real ans: %d\n", div16(n), div16_another(n), n / 16);
}
int main(int argv, char* args[]) {
    int n = 0x3 << 14;
    printf("%d", n * n);
    
}
