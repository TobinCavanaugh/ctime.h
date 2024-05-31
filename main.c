//
// Created by tobin on 5/30/2024.
//
#include <stdio.h>
#include "ctime.h"
#include "sw.h"

void printa() {
    puts("a\n");
    fflush(stdout);
}

void printb() {
    puts("b\n");
    fflush(stdout);
}

void printc() {
    puts("c\n");
    fflush(stdout);
}

void donothing(void) {
    printf("-\n");
}

int main() {
    sw_memory_print_auto();

    double ms = sw_start_ms();
    double us = sw_start_us();

    int i = 0;
//    for (; i < 10; i++) {
        ctime_create(2000, &donothing);
//    }


    while (!ctime_timers_stopped()) {}
    fflush(stdout);

    printf("%fus | %fms\n", sw_stop_us(us), sw_stop_ms(ms));

    sw_memory_print_auto();


    puts("DONE");

    return 0;
}
