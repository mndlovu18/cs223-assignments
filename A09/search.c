/*----------------------------------------------
 * Author:Minolta Ndlovu 
 * Date: 04/05/2023
 * Description: A program that uses N threads to 
 * search for a value in a list
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
  * A struct to hold the data for each thread.
*/
typedef struct {
    int* data;
    int start;
    int end;
    int target;
    int index;
} search_data_t;

/*
  * The thread function that searches for a value in a list.
  * @param arg A pointer to a search_data_t struct.
  * @return void*
*/

void *search_thread(void *arg) {
    search_data_t *search_thread = (search_data_t *) arg; // Cast the void* to a search_data_t*.

    for (int i = search_thread->start; i < search_thread->end; i++) {
        if (search_thread->data[i] == search_thread->target) { // If the value is found, set the 
                                                                  //index and exit the thread.
            search_thread->index = i;
            pthread_exit(NULL); // Exit the thread.
        }
    }

    search_thread->index = -1; // If the value is not found, set the index 
                                //to -1 and exit the thread.
    pthread_exit(NULL);

}

/* 
 * This is the main function. It creates N threads and 
 * searches for a value in a list.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 */
int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  int n_threads = atoi(argv[1]); // number of threads
  pthread_t *threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    search_data_t *thread_data = (search_data_t *) malloc(n_threads * sizeof(search_data_t));

  FILE* file = fopen("data.bin", "rb");
  if (!file) {
    printf("Error in open data bin file");
    return 1;
  }

  int size;
  fread(&size, sizeof(int), 1, file); // Read the size of the list from the file.
  int *data = malloc(sizeof(int) * size);
  fread(data, sizeof(int), size, file); // Read the list from the file.

  fclose(file);

  int target;
  printf("Enter a value to search for: ");
  scanf("%d", &target);

  int chunk_size = size / n_threads; // Calculate the chunk size.
  for ( int i = 0; i < n_threads; i++) {
    thread_data[i].data = data;
    thread_data[i].start = i * chunk_size; // Calculate the start index.
    thread_data[i].end = (i + 1) * chunk_size; // Calculate the end index.
    thread_data[i].target = target;
    thread_data[i].index = -1;

    pthread_create(&threads[i], NULL, search_thread, &thread_data[i]); // Create the thread.
  }

  int found = 0;
  for (int i = 0; i < n_threads; i++) {
      pthread_join(threads[i], NULL);
      if (thread_data[i].index != -1) {
          printf("Thread %d found %d at index %d\n", i + 1, target, thread_data[i].index);
          found = 1;
          break;
      }
  }

  if (!found) {
      printf("Not found!\n");
  }

  free(data);
  return 0;
}
