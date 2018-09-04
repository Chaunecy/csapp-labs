//#include<strlen.h>
#include<stdio.h>
#include<string.h>
int strlonger(char *s, char *t) {
    return strlen(s) > strlen(t);
}
int main() {
    char *s = "hello";
    char *t = "hello world";
    printf("s is longer %d\n", strlonger(s, t));
    s = "hello world wdt";
    
    printf("s is longer %d\n", strlonger(s, t));
    printf("hello world\n");
}

