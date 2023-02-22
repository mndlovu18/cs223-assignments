/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date: 02/07/2023
 * Description: A program thats reads text songs 
   into an array.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct song {  // a struct song to hold the song information.
    char title[128];
    char artist[128];
    int duration;
    float danceability;
    float energy;
    float tempo;
    float valence;
};

/**
* Prints a table of songs. 
* @param songs The array of songs
* @param num_songs The number of songs in the array
* @return void
*/
void print_songs(struct song *songs, int num_songs) {
    printf("Welcome to Dynamic Donna's Danceability Directory. \n");
    printf("\n");
    for (int i = 0; i < num_songs; i++) {
        printf("%d) %-30s artist: %-20s duration: %d:%02d D: %0.3f E: %0.3f T: %0.3f V: %0.3f \n",
            i, songs[i].title, songs[i].artist, songs[i].duration / 60000, (songs[i].duration % 60000) / 1000, 
            songs[i].danceability, songs[i].energy, songs[i].tempo, songs[i].valence);

    }
}

/**
 * Reads a CSV file of songs and prints them out.
 * @return int 
 */

int main(void) {
    FILE *songs = fopen("songlist.csv", "r"); // Open the file for reading.
    if (songs == NULL) {
        printf("Error opening file.");
        return 1;
    }
    
    char line[128]; // Read the number of songs from the first line.
    fgets(line, 128, songs);
    int num_songs = atoi(strtok(line, ","));

    char header[128]; // Read the header line.
    fgets(header, 128, songs);

    struct song *song_list = malloc(num_songs * sizeof(struct song)); // Allocate an array of songs using malloc
    int j = 0;
    while (fgets(line, 128, songs) != NULL) {
        struct song line_song; // Create a song struct for each line.
        char *token = strtok(line, ",");

        int i = 0;
        while (token != NULL) {
            if (i == 0) {
                strcpy(line_song.title, token);
            } else if (i == 1) {
                strcpy(line_song.artist, token);
            } else if (i == 2) {
                line_song.duration = atoi(token);
            } else if (i == 3) {
                line_song.danceability = atof(token);
            } else if (i == 4) {
                line_song.energy = atof(token);
            } else if (i == 5) {
                line_song.tempo = atof(token);
            } else if (i == 6) {
                line_song.valence = atof(token);
            }
            i++;
            token = strtok(NULL, ","); 
        }

        song_list[j] = line_song;  // Add the song struct to the array.
        j++;
    }
   print_songs(song_list, num_songs); //print all the songs in a table

   free(song_list); //free storage
   return 0;
}
