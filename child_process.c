#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id> \n");
        return 1;
    }
    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int n = atoi(argv[3]);  //store N as n
    int child_id = atoi(argv[4]);
    
    if(child_id >= (n-1) && child_id <= ((2*n)-2)){ //If the current process is a leaf process
        int block_file_number = child_id - (n-1);
        char block_file_number_str[12];

        sprintf(block_file_number_str,"%d",block_file_number);
        char* block_filename = malloc(strlen(blocks_folder)+1+strlen(block_file_number_str)+5);
        sprintf(block_filename,"%s/%d.txt",blocks_folder,block_file_number);
        char leaf_hash[SHA256_BLOCK_SIZE *2 + 1];
        hash_data_block(leaf_hash,block_filename);
        
        char* hash_file_name = malloc(strlen(hashes_folder) + 1 + strlen(argv[4])+5);
        sprintf(hash_file_name,"%s/%d.out",hashes_folder,child_id);
        
        FILE* hash_out = fopen(hash_file_name,"w");
        
        if(hash_out == NULL){
            fprintf(stderr,"Open Failed of : %s",hash_file_name);
            return 1;
        }
        fprintf(hash_out,"%s",leaf_hash);
        fclose(hash_out);
        
        free(hash_file_name);
        free(block_filename);
        
        return 0;

    }else if(child_id < n-1){
        
        int left_child_id = child_id * 2 + 1; //create left and right child ids from child_id
        int right_child_id = child_id * 2 + 2;
        
        char left_child_id_str[12]; //string for both child
        char right_child_id_str[12]; 
        
        sprintf(left_child_id_str, "%d", left_child_id);   //format to buffer
        sprintf(right_child_id_str, "%d", right_child_id);
        
        char *left_argv[] = {argv[0], blocks_folder, hashes_folder, argv[3], left_child_id_str, NULL};
        char *right_argv[] = {argv[0], blocks_folder, hashes_folder, argv[3], right_child_id_str, NULL};
        
        pid_t pid_left = fork();
        
        if(pid_left < 0){   // check for correct fork of left
            fprintf(stderr,"Fork Failed");
            return 1;
        } else if(pid_left == 0) { // <-- This is the child process
            if(execvp("./child_process",left_argv) < 0){    //check for correct exec
                perror("execvp failed");
                return 1;
            }
        }

        pid_t pid_right = fork();
        
        if(pid_right < 0){  // check for correct fork of right
            fprintf(stderr,"Fork Failed");
            return 1;
        } else if(pid_right == 0){ // <-- This is the child process
            if(execvp("./child_process",right_argv) < 0){   //check for correct exec
                perror("execvp failed");
                return 1;
            }
        }

        int status_left, status_right;
        if (waitpid(pid_left, &status_left, 0) == -1) { //check for wait success
            perror("waitpid failed");
            return 1;
        }

        if (waitpid(pid_right, &status_right, 0) == -1) { //check for wait success
            perror("waitpid failed");
            return 1;
        }  
        //write left hash
        char *left_hash_file_name = malloc(strlen(hashes_folder) + 1 + strlen(left_child_id_str) + 5);
        sprintf(left_hash_file_name,"%s/%d.out",hashes_folder,left_child_id);   //create hash file name
        
        FILE* left_hash_read = fopen(left_hash_file_name,"r");
        
        if(left_hash_read == NULL){ //check for file correctness
            fprintf(stderr,"Failed to open file %s  left\n", left_hash_file_name);
            return 1;
        }
        
        char left_hash[SHA256_BLOCK_SIZE * 2 + 1];  //setup left hash
        fread(left_hash,sizeof(char),SHA256_BLOCK_SIZE * 2,left_hash_read);
        left_hash[SHA256_BLOCK_SIZE * 2] = '\0';    //fill left hash
        //write right hash
        char *right_hash_file_name = malloc(strlen(hashes_folder) + 1 + strlen(right_child_id_str) + 5);
        sprintf(right_hash_file_name,"%s/%d.out",hashes_folder,right_child_id);
        
        FILE* right_hash_read = fopen(right_hash_file_name,"r");
        
        if(right_hash_read == NULL){    //check for file correctness
            fprintf(stderr,"Failed to open file %s right\n",right_hash_file_name);
            return 1;
        }
        
        char right_hash[SHA256_BLOCK_SIZE * 2 + 1]; //setup right hash
        fread(right_hash,sizeof(char),SHA256_BLOCK_SIZE * 2,right_hash_read);
        right_hash[SHA256_BLOCK_SIZE * 2] = '\0';
        //retrieve hashes to put to file result_hash
        char result_hash[SHA256_BLOCK_SIZE * 2 + 1];
        compute_dual_hash(result_hash, left_hash, right_hash);
        char* hash_file_name_par = malloc(strlen(hashes_folder) + 1 + strlen(argv[4])+5);
        sprintf(hash_file_name_par,"%s/%d.out",hashes_folder,child_id);
        
        FILE* hash_out_par = fopen(hash_file_name_par,"w");
        
        if(hash_out_par == NULL){   //confirm file open
            fprintf(stderr,"Open Failed of : %s",hash_file_name_par);
            return 1;
        }
        
        fprintf(hash_out_par,"%s",result_hash); //print to file the concatenated hash
        fclose(hash_out_par);
        
        free(hash_file_name_par);
        free(right_hash_file_name);
        free(left_hash_file_name);
        return 0;
    }
    return 0;
}
    

