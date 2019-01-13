#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rpi.h"
#include "bitwise.h"

#define NPINS 8

//pin lato sinistro (3v top)
#define EGG0 4
#define EGG1 17
#define EGG2 27
#define MOV0 22
#define MOV1 10
#define ORD0 9
#define ORD1 11
#define ORD2 5

int main() {

        int fd_down[2 * NPINS];  // 2-way file descriptor (aka pipe) per ogni pin

        int pins[NPINS] = {EGG0, EGG1, EGG2, MOV0, MOV1, ORD0, ORD1, ORD2};
        pid_t pids[NPINS];
        int status;

        for (int i = 0; i < NPINS; i++) {
                pids[i] = fork();
                pipe(&fd_down[2 * i]);   // la pipe non sa che l'array è più grande e si ferma ai due canali [i] e [i+1]

                if (pids[i] < 0) {        //error
                        perror("fork error at output");
                        abort();

                } else if (pids[i] == 0) {        // WARNING: CHILD -> l'indice i persiste !!
                        printf("output child %d \n",i);
                        char res[1];
                        int status;
                        GPIOExport(pins[i]);
                        GPIODirection(pins[i],'o');
                        close(fd_down[2*i+1]);     // chiudo IL descriptor di write
                        while(1) {
                                read(fd_down[2*i], res, 1);
                                printf("output child %d legge valore %c\n",i,res[0]);
                                GPIOWrite(pins[i],getBit(i,res[0])); // scrivo valore pin
                                sleep(1);
                        }
                        close(fd_down[2*i]);
                        exit(0);
                }
        }

        // WARNING: DAD


        char * downstream = "/tmp/downstream";
        int fd_up = open(downstream, O_RDONLY); // comunica col principale

        for(int k=0; k<NPINS; k++) {
                close(fd_down[2*k]); // chiudo I descriptor read verso il basso
        }

        while(1) {
                char result[1];
                char data[1] = {0b00000000};
                read(fd_up, result, 1);         // leggo ciò che scende
                for(int j=0; j<NPINS; j++) {
                        if(getBit(j,result[0])) {
                                setBit(j,data[0]);
                        }
                }

                for(int p=0; p<NPINS; p++) {
                        data[0] = getBit(p,data[0]);
                        write(fd_down[2*p], data, 1); // e lo mando ai pin
                }
                printf("%c\n", data[0]);
                sleep(1);
        }



        /* funzione probabilmente utile per il debug
           int status;
           pid_t pid;
           while (NPINS > 0) {
                pid = wait(&status);
                printf("Child with PID %ld exited with status 0x%x.\n", (long) pid, status);
                NPINS--;
           }
         */
        exit(0);



}
