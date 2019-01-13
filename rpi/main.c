#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitwise.h"


int main(){
        char base = 0b00000000;
        int fd_up[2*2];
        int fd_down[2];
        char * upstream = "/tmp/upstream";
        char * downstream = "/tmp/downstream";

        mkfifo(upstream, 0666);
        mkfifo(downstream, 0666);

        system("./input");
        system("./output");

        pid_t pid = fork();
        int status;


        if(pid < 0) { // niet
          perror("fork error at main");
          abort();
        } else if(pid == 0) { // gestore dei miei stivali

                while(1) {
                        int uova = 0;
                        int mag = 0;
                        int ordina = 0;
                        int sposta = 0;

                        char bufin[1];
                        char bufout[1];

                        // leggi quello che sale
                        fd_down[0] = open(upstream, O_RDONLY);
                        read(fd_down[0], bufin, 1);
                        printf("MAIN legge %c\n", bufin);

                        // conta n uova e magazzino
                        for(int i = 0; i<8; i++) {
                                if(getBit(i,bufin[0])) {
                                        if(i<6) {
                                                uova++;
                                        }else{
                                                if(i==6 && getBit(i,bufin[0])) {
                                                        mag++;
                                                }else if (i==7 && getBit(i,bufin[0])) {
                                                        mag = mag + 2;
                                                }
                                        }
                                }
                        }
                        printf("MAIN conta: %d %d\n", uova, mag);
                        // scrivi n uova
                        for(int j=0; j<3; j++) {
                                if(getBit(j,uova+'0')) {
                                        setBit(j,bufout[0]);
                                }
                        }
                        // scrivi n ordine
                        if(uova < 6) {
                                ordina = 6 - uova;
                                for(int k=5; k<7; k++) {
                                        if(getBit(k,ordina + '0')) {
                                                setBit(k,bufout[0]);
                                        }
                                }
                        }
                        // scrivi n sposta
                        if(ordina > 0) {
                                sposta = ordina%3;
                                if(sposta == 0) {
                                        sposta = 3;
                                }
                                for(int l=3; l<5; l++) {
                                        if(getBit(l,sposta + '0')) {
                                                setBit(l,bufout[0]);
                                        }
                                }
                        }
                        // manda sotto
                        printf("MAIN scrive: %d %d su bufout: %c\n", ordina, sposta, bufout);
                        fd_down[1] = open(downstream, O_WRONLY);
                        write(fd_down[1], bufout, 1);

                        sleep(3);
                }
                close(fd_down[0]);
                close(fd_down[1]);
                unlink(upstream);
                unlink(downstream);
                exit(0);

        } else { // gestore generale


        }

}
