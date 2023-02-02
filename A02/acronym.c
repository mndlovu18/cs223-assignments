#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
  char buff[1024];
  printf("Enter a phrase: ");
  fgets(buff, 1024, stdin); // read an entire string from the user
  
  printf("Your acronym is ");
  int buff_len = strlen(buff);
  for (int i = 0; i <  buff_len; i++) {
      if (buff[i] >= 'A' && buff[i] <= 'Z') {
          printf("%c", buff[i]);
      }

      //add a newline at the end
      if (i == buff_len - 1) {
        printf("\n");
      }
  }
}
