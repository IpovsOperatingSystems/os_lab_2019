//
// Created by sergeyampo on 24.04.2020.
//
#include <unistd.h>

int main(){
    printf("Now we're running sequential_min_max in exec mode!\n");
    execlp("MietOSSequentially", "MietOSSequentially", 100500, 1234, 100500);

    return 0;
}