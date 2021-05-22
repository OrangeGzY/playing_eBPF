#include<stdio.h>
#include<stdlib.h>

void test(){


    printf("Hello world\n");
}

int test2(int a,char b,char *c){
    printf("a:%d\nb:%c\nc:%s\n",a,b,c);
    return -100;
}
int test3(int a){

    a = a+1;
    return a;
}
int main(int argc, char **argv){
    test3(2);
    test();
    test2(0x10, 'f', "bpf uprobe");
    return 0;
}
