/*----------------------------------------------
 * Author:Minolta Ndlovu
 * Date: 02/22/2023
 * Description:A program that generates a crossword
 * using two words from a user.
 ---------------------------------------------*/

#include <stdio.h> // printf, scanf
#include <stdlib.h> // malloc, free
#include <string.h> // strlen


/**
 * @brief The main driver program for the crossword puzzle
 *
 *This program will take two words from a user and
 *create a crossword puzzle
 *
 * @param argc number of arguments
 * @param argv the arguments from a user
 * @return int
 */
int main(int argc, char *argv[]) {
    if (argc != 3) { //check for correct number of arguments
        printf("Usage: %s <word1> <word2>\n", argv[0]);
        return 1;
    }

    char *word1 = argv[1];
    char *word2 = argv[2];
    int word1_len = strlen(word1);
    int word2_len = strlen(word2);
    
    int common_index = -1; //keep track of the 
                           //index of the common letter in word1
    for (int i = 0; i < word1_len; i++) {
        for (int j = 0; j < word2_len; j++) {
            if (word1[i] == word2[j]) {
                common_index = i;
                break;
            }
        }

        if (common_index != -1) {
            break;
        }
    }

    if (common_index == -1) {
        printf("No common letter!\n"); //if no common letter, exit
        return 1;
    }

    char **crossword = (char **) malloc(word2_len * 
        sizeof(char *)); //allocate memory for the crossword
    for (int i = 0; i < word2_len; i++) {
        crossword[i] = (char *) malloc(word1_len * 
            sizeof(char));
    }

    for (int i = 0; i < word2_len; i++) {
        for (int j = 0; j < word1_len; j++) {
            crossword[i][j] = '.'; //initialize the crossword to '.'
        }
    }

    for (int i = 0; i < word1_len; i++) {
        if (i == common_index) {
            for (int j = 0; j < word2_len; j++) {
                crossword[j][i] = word2[j]; //add word2 to the crossword
            }
        } else {
            crossword[word2_len - 1][i] = word1[i]; //add word1 to the crossword
        }
    }

    for (int i = 0; i < word2_len; i++) {
        for (int j = 0; j < word1_len; j++) {
            printf("%c ", crossword[i][j]); //print the crossword
        }
        printf("\n");
    }

    for (int i = 0; i < word2_len; i++) {
        free(crossword[i]); //free the memory
    }

    free(crossword);

    return 0;
}

