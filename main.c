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

int v = 0;

void donothing(void) {
//    printf("~");
//    int i = 0;
//    i += 100;
    ++v;
    printf("-");
}

int main() {
    sw_memory_print_auto();

    double ms = sw_start_ms();
    double us = sw_start_us();

    int i;
    for (i = 0; i < 1000; i++) {
        create_timer(1000, &donothing);
    }

    while (!all_timers_stopped()) {
//        printf("%d\n", ctime_active_timers_count);
    }

    printf("%d\n\n", v);

    printf("%fus | %fms\n", sw_stop_us(us), sw_stop_ms(ms));
    sw_memory_print_auto();

    puts("DONE");

    return 0;
}
