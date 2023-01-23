/***************************************************
* mathgame.c
 * Author:Minolta Ndlovu 
 * Implements a math game
 */

#include <stdio.h>

int main() {
    printf("Welcome to Math Game!/n");
    printf("How many rounds do you want to play? ");

    int rounds;
    scanf("%d/n", &rounds);

    int correct = 0;

    for (int i = 0; i < rounds; i++) {
        int a = rand() % 9 + 1;
        int b = rand() % 9 + 1;
        int answer = a + b;

        printf("%d + %d = ? ", a, b);

        int user_answer;
        scanf("%d", &user_answer);

        if (user_answer == answer) {
            printf("Correct!");
            correct++;
        } else {
            printf("Incorrect :(");
        }

    }

    printf("You answered %d/%d correctly.", correct, rounds);

}

