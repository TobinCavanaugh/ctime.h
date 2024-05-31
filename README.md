# ctime.h
### A simple non-blocking C timer in a single header file

`ctime.h` is an easy single header library to handle timers in pure C. The core of the idea is to have non-blocking timers for applications that don't necessarily have a main function loop.

The process of using a timer is to:
- Create a thread
- Wait `n`ms in the thread
- Call the callback function


`ctime.h` has been tested with hundreds of timers running at once without any issues. To my understanding there is no issues with memory allocation, and I handle all threads and memory correctly.

### Gif Example:
![crop](https://github.com/TobinCavanaugh/ctime/assets/71297845/53f0ca48-e8bf-406f-bfbd-f45d464066bf)

<details>
<summary>Code</summary>
  
```C
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
```
</details>


### Code Example:
```C
void print_thing(void){
  puts("timer up");
}

int main(){
  //Start the timer for 1s (1000ms)
  ctime_start(1000, &print_thing);

  //(Optional) Prevent the program from ending before all timers are stopped.
  while (!ctime_timers_stopped()) {}

  return 0;
}
```

