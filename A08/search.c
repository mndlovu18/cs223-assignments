#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
* Searches for a value in the array and prints the result.
* If start is 0, then this is the parent process.
* Otherwise, this is the child process.
* @param data The array to search
* @param size The size of the array
* @param value The value to search for
* @param start The start index to search from
* @param end The end index to search to
*/
void search(int *data, int size, int value, int start, int end) {
    for (int i = start; i < end; i++) {
        if (data[i] == value) {
            printf("%s found %d at index %d\n", start == 0 ? "Parent" : "Child", value, i);
            return;
        }
    }
    printf("%s not found!\n", start == 0 ? "Parent" : "Child");
}

/*
* Reads an array of integers from a file and searches for a value.
* The first integer in the file is the size of the array.
* @param argc The number of command line arguments
* @param argv The command line arguments
* @return 0 if successful, 1 otherwise
*/
int main() {
    int value;
    printf("Enter a value to search: ");
    scanf("%d", &value);

    FILE* fp = fopen("data.bin", "rb");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    int size;
    fread(&size, sizeof(int), 1, fp);

    int *data = malloc(size * sizeof(int));
    fread(data, sizeof(int), size, fp);
    fclose(fp);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error in fork");
        free(data);
        return 1;
    }

    if (pid == 0) {
        // Child process
        search(data, size, value, size / 2, size);
    } else {
        // Parent process
        search(data, size, value, 0, size / 2);
        wait(NULL);
    }

    free(data);
    return 0;
}

