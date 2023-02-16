/*----------------------------------------------
 * Author:Minolta Ndlovu
 * Date: 02/15/2023
 * Description: A program that allows users to
   easily access the most danceable songs in the
   songlist.csv.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024 // 1024 bytes of data per line
#define MAX_TITLE_LEN 256 // 256 bytes of data for title
#define MAX_ARTIST_LEN 256 // 256 bytes of data for artist

typedef struct Song { // Define a struct for a song
    char title[MAX_TITLE_LEN];
    char artist[MAX_ARTIST_LEN];
    int duration_ms;
    float danceability;
    float energy;
    float tempo;
    float valence;
    struct Song *next;
} Song; // Define Song to avoid having to type struct song everywhere

/**
 * @brief Inserts a song into the linked list
 * @param head Pointer to the head of the linked list
 * @param song_index The index of the song to insert
 * @param title The title of the song
 * @param artist The artist of the song
 * @param duration_ms The duration of the song in milliseconds
 * @param danceability The danceability of the song
 * @param energy The energy of the song
 * @param tempo The tempo of the song
 * @param valence The valence of the song
 */
void insert_song(Song **head, int song_index, char *title, char *artist, 
int duration_ms, float danceability, float energy, float tempo, float valence) {
    Song *new_song = malloc(sizeof(Song)); // Allocate memory for the new song
    if (new_song == NULL) {
        printf("Error: Could not allocate memory for new song");
        exit(1);
    }
    strcpy(new_song->title, title);
    strcpy(new_song->artist, artist);
    new_song->duration_ms = duration_ms;
    new_song->danceability = danceability;
    new_song->energy = energy;
    new_song->tempo = tempo;
    new_song->valence = valence;
    new_song->next = NULL;
    if (song_index == 0) {
        new_song->next = *head; //next is set to value of head
        *head = new_song; //head is set to value of new_song
    } else {
        Song *current = *head;
        Song *previous = NULL;
        int i = 0;
        while (i < song_index) {
            previous = current;
            current = current->next;
            i++;
        }
        new_song->next = current;
        previous->next = new_song;
    }

}

/**
 * @brief Prints a table of songs to the console
 * @param head Pointer to the head of the linked list
 * @param song_count The current number of the song in the list
*/ 
void print_song(Song *song, int song_count) {
    int minutes = song->duration_ms / 60000;
    int seconds = (song->duration_ms % 60000) / 1000;
    printf("%d) %-30s artist: %-20s duration: %d:%02d D: %0.3f E: %0.3f T: %0.3f V: %0.3f\n",
            song_count, song->title, song->artist, minutes, seconds, song->danceability, song->energy, song->tempo, song->valence);
}

/**
 * @brief Finds the most danceable song in the linked list
 * @param head Pointer to the head of the linked list
 * @return Pointer to the most danceable song
 */
Song *find_most_danceable(Song *head) {
    Song *current = head;
    Song *most_danceable = head;
    while (current != NULL) {
        if (current->danceability > most_danceable->danceability) {
            most_danceable = current;
        }
        current = current->next;
    }
    return most_danceable;
}

/**
 * @brief Removes a song from the linked list
 * @param head Pointer to the head of the linked list
 * @param song_index The index of the song to remove
 */
void remove_song(Song **head, int song_index) {
    Song *current = *head;
    Song *previous = NULL;
    int i = 0;
    while (i < song_index) {
        previous = current;
        current = current->next;
        i++;
    }
    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
}

/**
 * @brief Free memory for all songs in the linked list
 * @param head Pointer to the head of the linked list
 */
void free_songs(Song **head) {
    Song *current = *head;
    Song *next = NULL;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

/**
*Reads CSV data from a file and inserts it into
*a linked list
*/
int main() {
  FILE *songs = fopen("songlist.csv", "r"); // Open the file for reading.
    if (songs == NULL) {
        printf("Error opening file.");
        return 1;
    }
  Song *head = NULL; //Initialize head pointer

  char line[MAX_LINE_LEN]; // Buffer for reading lines from the file
  fgets(line, MAX_LINE_LEN, songs); // Skip the header line
  int song_index = 0;
  while (fgets(line, MAX_LINE_LEN, songs) != NULL) {
        Song *new_song = malloc(sizeof(Song)); // Allocate memory for a new song

        if (new_song == NULL) {
            printf("Error allocating memory for song");
            fclose(songs);
            return 1;
        }

        char *token = strtok(line, ","); // Get the first token
        int i = 0;
        while (token != NULL) {
            if (i == 0) {
                strcpy(new_song->title, token);
            } else if (i == 1) {
                strcpy(new_song->artist, token);
            } else if (i == 2) {
                new_song->duration_ms = atoi(token);
            } else if (i == 3) {
                new_song->danceability = atof(token);
            } else if (i == 4) {
                new_song->energy = atof(token);
            } else if (i == 5) {
                new_song->tempo = atof(token);
            } else if (i == 6) {
                new_song->valence = atof(token);
            }
            i++;
            token = strtok(NULL, ",");
        }

        insert_song(&head, song_index, new_song->title, new_song->artist,
        new_song->duration_ms,new_song->danceability, new_song->energy,
        new_song->tempo, new_song->valence);

        song_index++;
  }
    while (1) {
         int num_songs = 0;
         Song *current = head;
         while (current != NULL) {
           print_song(current, num_songs);
           num_songs++;
           current = current->next;
        }

        printf("\nDataset contains %d songs\n\n", num_songs);

        printf("=======================\n");
        printf("Press 'd' to show the most danceable song (any other key to quit):");
        char input = getchar();
        while (getchar() != '\n'); // Clear the input buffer

        if (input != 'd') {
            free_songs(&head);// Free the memory for all songs
            break;
        }  else {


            if (head == NULL) {
                continue; // check if there are any songs in the list
            }

            Song *most_danceable = find_most_danceable(head);
            printf("--------------------------------------- Most danceable -------------------------------------------------------------\n");
            print_song(most_danceable, 0);
            printf("--------------------------------------------------------------------------------------------------------------------\n");
            printf("\n");

            int song_index = 0;//find the song_index of the most danceable song
            Song *current = head;
            while (current != NULL) {
                if (current == most_danceable) {
                    break;
                }
                song_index++;
                current = current->next;
            }

            remove_song(&head, song_index); // Remove the song from the linked list
            num_songs--;
        }
    }


  return 0;
}
