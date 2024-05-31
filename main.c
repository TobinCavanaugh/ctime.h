//
// Created by tobin on 5/30/2024.
//
#include <stdio.h>
#include "ctime.h"

void printa() {
    printf("a\n");
    fflush(stdout);
}

void printb() {
    printf("b\n");
    fflush(stdout);
}

void printc() {
    printf("c\n");
    fflush(stdout);
}

void donothing(void) {

}

int main() {
    int i = 0;

//    while(1){
    for (; i < 1000; i++) {
        create_timer(1000, &printa);
        create_timer(750, &printb);
        create_timer(500, &printc);
    }

    while (ctime_active_timers > 0) {}

    printf("DONE");

    return 0;
}
