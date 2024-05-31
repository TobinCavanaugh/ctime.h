//
// Created by tobin on 5/30/2024.
//
#include <stdio.h>
#include "ctime.h"

void printa() {
    puts("3s timer up");
}

void printb() {
    puts("1s timer up");
}

void printc() {
    puts("100ms timer up");
}

void clear() {
    system("cls");
}

int main() {

    system("pause");

    ctime_start(3000, &printa);
    ctime_start(1000, &printb);
    ctime_start(100, &printc);
    ctime_start(5000, &clear);

    while (!ctime_timers_stopped()) {}

    return 0;
}