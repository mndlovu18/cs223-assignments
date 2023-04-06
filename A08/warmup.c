/*----------------------------------------------
 * Author: Minolta Ndlovu
 * Date: 03/24/2023
 * Description: This program spawns processes A, B, and C
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * This function spawns processes A, B, and C
 * @return: 0 if successful
*/
int main() {
    pid_t pid_A, pid_B; 

    printf("%d] A0\n", getpid()); 

    pid_A = fork();

    if (pid_A > 0) {
        printf("%d] BO\n", getpid()); //if parent, print B0
    }

    if (pid_A == 0) {
        printf("%d] B1\n", getpid()); //if child, print B1
        printf("%d] Bye\n", getpid()); //if child, print Bye
        exit(0);
    }

    
    pid_B = fork(); //fork again


    if (pid_B > 0) {
        printf("%d] C0\n", getpid()); //if parent, print C0
        printf("%d] Bye\n", getpid()); //if parent, print Bye
        exit(0);
    }

    if (pid_B == 0) {
        printf("%d] C1\n", getpid()); //if child, print C1
        printf("%d] Bye\n", getpid()); //if child, print Bye
        exit(0);
    }

    return 0;
}


