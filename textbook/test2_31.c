#include<stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 3) {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);
    }
    printf("argc = %d", argc);
}

int tadd_ok(int x, int y) {
    int sum = x + y;
    if (x < 0 && y < 0 && sum >= 0) {
        return 0;
    }
    else if (x > 0 && y > 0 && sum <= 0) {
        return 0;
    }
    else
        return 1;
} 
