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

    int i;
    for (i = 0; i < 10; i++) {
        ctime_create(1000, &donothing);
    }

    while (!ctime_timers_stopped()) {}

    printf("%fus | %fms\n", sw_stop_us(us), sw_stop_ms(ms));
    sw_memory_print_auto();

    pthread_t thread;

    pthread_create(&thread, NULL, NULL, NULL);
    printf("%lld\n", thread);

    puts("DONE");

    return 0;
}
