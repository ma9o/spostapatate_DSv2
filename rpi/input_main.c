#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi.h"
#include "bitwise.h"

#define NPINS 8

//pin lato desto (5v top)
#define EGG0 23
#define EGG1 24
#define EGG2 25
#define EGG3 8
#define EGG4 7
#define EGG5 4
#define MAG0 12
#define MAG1 16

int main()
{

    int fd_down[2 * NPINS]; // 2-way file descriptor (aka pipe) per ogni pin

    int pins[NPINS] = { EGG0, EGG1, EGG2, EGG3, EGG4, EGG5, MAG0, MAG1 };
    pid_t pids[NPINS];
    int status;

    for (int i = 0; i < NPINS; i++) {
        pids[i] = fork();
        pipe(&fd_down[2 * i]); // la pipe non sa che l'array è più grande e si ferma ai due canali [i] e [i+1]

        if (pids[i] < 0) { //error
            perror("fork error at input");
            abort();
        }
        else if (pids[i] == 0) { // WARNING: child -> l'indice i persiste !!

            char res[1];
            GPIOExport(pins[i]);
            GPIODirection(pins[i], 'i');
            close(fd_down[2 * i]); // chiudo IL descriptor di read
            while (1) {
                GPIORead(pins[i], res); // leggo valore pin
                printf("input child %d legge valore %c\n", i, res[0]);
                write(fd_down[2 * i + 1], res, 1);
                sleep(1);
            }
            close(fd_down[2 * i + 1]);
            exit(0);
        }
    }
    // WARNING: DAD

    char* upstream = "/tmp/upstream";
    int fd_up = open(upstream, O_WRONLY);

    for (int k = 0; k < NPINS; k++) {
        close(fd_down[2 * k + 1]); // chiudo I descriptor di write verso il basso
    }

    while (1) {
        char result[1];
        char data[1] = { 0b00000000 };
        for (int j = 0; j < NPINS; j++) {
            read(fd_down[2 * j], result, 1); // leggo ciò che sale
            if (getBit(j, result[0])) {
                setBit(j, data[0]);
            }
        }
        write(fd_up, data, 1); // e lo mando al gestore
        printf("%c\n", data[0]);
        sleep(1);
    }

    exit(0);
}
