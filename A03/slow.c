/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date: 02/08/2022
 * Description: This program takes a string and adds pauses between each letter
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* Turns a phrase into ent-speech
* @return int
*/
int main(void) {
    int pause;
    char buff[32];
    char *ent;
    int i, j, k;

    printf("Pause length: ");
    scanf("%d", &pause);
    printf("Text: ");
    scanf("%s", buff);

    ent = malloc(strlen(buff) * pause + strlen(buff) + 1); //a malloc string that can hold the ent speech with pauses (len(string) * pause + len(string))


    for (i = 0, j = 0; i < strlen(buff); i++, j++) {
        if (i == 0) {
            ent[j] = buff[i];
            continue; //if its the first letter, dont add a pause
        }

        for (k = 0; k < pause; k++, j++) {
            ent[j] = '.'; //add the pause
        }
        ent[j] = buff[i];
    }

    ent[j] = '\0';

    printf("%s \n", ent);

    free(ent);

    return 0;

}
