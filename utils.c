#include "utils.h"
#include "math.h"
// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    FILE *fp = fopen(input_file, "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    long partition_size = size/n;
    rewind(fp); // reset file pointer to the beginning

    char *buffer = (char*)malloc(partition_size + 1);
    for(int i = 0; i < n-1; i++){
        char output_file[256];
        sprintf(output_file, "%s/%d.txt", blocks_folder, i); // create the output file path
        FILE *out = fopen(output_file, "w");
        if(out == NULL) {
            printf("Failed to open file: %s\n", output_file);
            continue;
        }

        size_t read_size = fread(buffer, 1, partition_size, fp);
        fwrite(buffer, 1, read_size, out); // write to the output file
        fclose(out);
    }
    char output_fileL[256];
    int last_name = n-1;
    long new_part = (size/n) + (size % n);
    buffer = realloc(buffer, new_part + 1); // reallocate the buffer
    if(buffer == NULL) {
    printf("Failed to allocate memory\n");
    return;
    }
    sprintf(output_fileL, "%s/%d.txt", blocks_folder, last_name); // create the output file path
    FILE *out_last = fopen(output_fileL, "w");
    if(out_last == NULL) {
    printf("Failed to open file: %s\n", output_fileL);
    }
    size_t read_size_last = fread(buffer, 1, new_part, fp);
    fwrite(buffer, 1, read_size_last, out_last); // write to the output file
    fclose(out_last);
    free(buffer);
    fclose(fp);
}


// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder) {
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0) {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0) {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0) {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}
