#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  A program that fills an array with three or more songs and 
  then allows the user to edit the data in the array.
*/

//Struct to hold songs
struct Song {
   char title[1024];
   char artist[1024];
   int duration_min;
   int duration_sec;
   float danceability;
};

/* printSongs: prints the songs in the array
 * songs[]: an array of songs
 * numSongs: the number of songs in array
 */
void printSongs(struct Song songs[], int numSongs);

/* main: allows the user to view and edit songs */
int main() {
  //Initialize songs
  struct Song songs[3]; //an array of songs
  strcpy(songs[0].title, "Shout");
  strcpy(songs[0].artist, "Tears for Fears");
  songs[0].duration_min = 4;
  songs[0].duration_sec = 11;
  songs[0].danceability = 0.50;
  
  strcpy(songs[1].title, "As it was");
  strcpy(songs[1].artist, "Harry Styles");
  songs[1].duration_min = 2;
  songs[1].duration_sec = 47;
  songs[1].danceability = 0.70;

  strcpy(songs[2].title, "Wish you were here");
  strcpy(songs[2].artist, "Pink Floyd");
  songs[2].duration_min = 5;
  songs[2].duration_sec = 34;
  songs[2].danceability = 0.30;

  printf("Welcome to Steven Struct's Song List. \n");
  printf("\n");

  printSongs(songs, 3);
  printf("\n");

  //Print message to edit song
  printf("\n");
  printf("Enter a song id to edit [0,1,2]: ");

  int songId;
  scanf("%d", &songId);

  // Check for valid song id
   if (songId < 0 || songId > 2) {
        printf("Invalid choice!");
        printf("\n");
        return 0;
    }

  printf("\n");
  printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
  char attribute[32];
  scanf("%s", attribute);
  printf("\n");

  if (strcmp(attribute, "artist") == 0) {
        printf("Enter a artist: ");
        char input[1024];
        scanf("%s*c", input);
        strcpy(songs[songId].artist, input);
        printSongs(songs, 3);
    } else if (strcmp(attribute, "title") == 0) {
        printf("Enter a title: ");
        char title[1024];
        scanf("%s*c", title);
        strcpy(songs[songId].title, title);
        printSongs(songs, 3);
    } else if (strcmp(attribute, "duration") == 0) {
        printf("Enter a duration (minutes): ");
        int duration_min;
        scanf("%d*c",&duration_min);
        printf("Enter a duration (seconds): ");
        int duration_sec;
        scanf("%d*c", &duration_sec);
        songs[songId].duration_min = duration_min;
        songs[songId].duration_sec = duration_sec;
        printSongs(songs, 3);
    } else if (strcmp(attribute, "danceability") == 0) {
        printf("Enter danceability: ");
        float danceability;
        scanf("%f*c", &danceability);
        songs[songId].danceability = danceability;
        printSongs(songs, 3);
    } else {
        printf("Invalid choice!");
    }
  return 0;
}

/* printSongs: prints the songs in the array
 * songs[]: an array of songs
 * numSongs: the number of songs in array
 */
void printSongs(struct Song songs[], int numSongs) {
    for (int i = 0; i < numSongs; i++) {
        printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f ", i, songs[i].title, songs[i].artist, songs[i].duration_min, songs[i].duration_sec, songs[i].danceability);
        printf("\n");
    }
    printf("\n");
    printf("============================== \n");
}
