#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* This is a program that turns a word
 * into an exclaimation.
 */

/* main : prints an exclamation given
 * a word.
 */

int main() {
  char buff[32];
  printf("Enter a word: ");
  scanf("%s", buff); //read a string from user
  printf("OMG! ");
  srand(time(NULL)); //seed the random generator
 
 int buff_len = strlen(buff);
  for (int i = 0; i < buff_len; i++) {
     if (buff[i] >= 'a' && buff[i] <= 'z') {  // check if the character is a lowercase letter
         int random_ = rand() % 4;
         if (random_ == 0) {      
            printf("@");
         } else if (random_ == 1) {
            printf("!");
         } else if (random_ ==2) {
            printf("#");
         } else {
            printf("*");
         }
     } else {
        printf("%c", buff[i]); //else print the character
     }
     
     //add a newline at the end
    if (i == buff_len - 1) {
        printf("\n");
    }
  }
  return 0;
}   
