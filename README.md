Group 12
Cade Roggenbauer rogge056@umn.edu and Guy Fleischhacker fleis215@umn.edu
Used csel-kh1250-01.cselabs.umn.edu for testing
Cade- General planning of psuedocode, recursive function design, utils,and readability
Guy- Merkle, child_process, partitioning function, and bug fixing

Psuedocode:
1. Open the input file for reading.
2. Calculate the size of the input file.
3. Calculate the partition size by dividing the file size by the number of blocks (n).
4. Allocate a buffer for reading and storing partition data.
5. For each block (i) from 0 to n-1:
    a. Create an output file for the i-th block.
    b. Read partition_size bytes from the input file into the buffer.
    c. Write the data from the buffer to the output file.
6. For the last block (n-1):
    a. Adjust the buffer size to accommodate the remaining data.
    b. Create an output file for the last block.
    c. Read the remaining data from the input file into the buffer.
    d. Write the data from the buffer to the output file.
7. Free memory allocated for the buffer.
8. Close the input file.

**merkle.c:**
1. Parse command-line arguments for the input file and the number of blocks (N).
2. Call setup_output_directory to create the necessary output directories.
3. Call partition_file_data to split the input file into N blocks.
4. Fork a child process to start the recursive Merkle tree computation (root).
5. In the parent process:
    a. Wait for all child processes to finish.
    b. Optionally print the Merkle tree structure to a visualization file.

**child_process.c:**
1. Parse command-line arguments for the blocks folder, hashes folder, N, and child_id.
2. If child_id is in the range of leaf processes (n-1 to 2*n-2):
    a. Calculate the block file number for the leaf process.
    b. Compute the hash of the corresponding block.
    c. Write the hash to a hash file in the hashes folder.
3. If child_id is in the range of non-leaf processes (0 to n-2):
    a. Calculate left and right child IDs.
    b. Fork two child processes for left and right children.
    c. In each child process:
        i. Recursively call this child process with the appropriate arguments.
    d. Wait for both child processes to finish.
    e. Read the hashes of left and right children from hash files.
    f. Compute the hash of the current node using the hashes of left and right children.
    g. Write the computed hash to a hash file in the hashes folder.

AI was used to write debugging statements that have since been deleted in order to debug the code such as outputing to the system when an error is encountered. This was done because we were stuck bug fixing for code that we were sure would functionally worked, just needed to find where we were encoutering an issue.
