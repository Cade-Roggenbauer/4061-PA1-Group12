#include "utils.h"
#include "print_tree.h"

// ##### DO NOT MODIFY THESE VARIABLES #####
char *blocks_folder = "output/blocks";
char *hashes_folder = "output/hashes";
char *visualization_file = "output/visualization.txt";


int main(int argc, char* argv[]) {
    if (argc != 3) {
        // N is the number of data blocks to split <file> into (should be a power of 2)
        // N will also be the number of leaf nodes in the merkle tree
        printf("Usage: ./merkle <file> <N>\n");
        return 1;
    }

    //Read in the command line arguments and validate them
    char *input_file;
    int n;
    input_file = argv[1];
    n = atoi(argv[2]);

    // ##### DO NOT REMOVE #####
    setup_output_directory(blocks_folder, hashes_folder);

    //partition the input file into the blocks folder
    partition_file_data(input_file, n, blocks_folder);


    //Start the recursive merkle tree computation by spawning first child process (root)
    int status;
    pid_t pid = fork();
    if(pid == 0){
    char* argv_new[] = {argv[0],blocks_folder,hashes_folder,argv[2],"0\0",NULL};
    if(execvp("./child_process",argv_new) < 0){
        printf("huh???\n");
        return 1;
    }
    }else{
    // Wait for all child processes to finish
    while ((pid = wait(&status)) > 0);
    // ##### DO NOT REMOVE #####
    #ifndef TEST_INTERMEDIATE
    // Visually display the merkle tree using the output in the hashes_folder
    print_merkle_tree(visualization_file, hashes_folder, n);
    #endif
}
    return 0;
}