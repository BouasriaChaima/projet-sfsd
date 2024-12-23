#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 100      // Nbr totale des blocs
#define BLOCK_SIZE 512     // Taille de chq bloc (en bytes)

//  on le fait comme un debut
// Structure du bloc
typedef struct {
    int isFree;            // 1 if block is free (c auto), 0 if used
    char data[BLOCK_SIZE]; // Content of the block
} Block;

// structure du disque
typedef struct {
    Block blocks[DISK_SIZE]; // Array of blocks
    int freeBlocks;          // Nbr of free blocks
} Disk;

// Function to initialize the disk
void initializeDisk(Disk *disk) {
    disk->freeBlocks = DISK_SIZE; // Initially tous les blocs sont free

    // Set all blocks to free
    for (int i = 0; i < DISK_SIZE; i++) {
        disk->blocks[i].isFree = 1;
        memset(disk->blocks[i].data, 0, BLOCK_SIZE); // Clear block data
    }

    printf("Disk initialized with %d blocks, each of size %d bytes.\n", DISK_SIZE, BLOCK_SIZE);
}

// Function to display the current status of the disk
void displayDiskStatus(Disk *disk) {
    printf("Total Blocks: %d\n", DISK_SIZE);
    printf("Free Blocks: %d\n", disk->freeBlocks);
    printf("Used Blocks: %d\n", DISK_SIZE - disk->freeBlocks);
}

int main() {
    Disk myDisk;

    // Initialize the disk
    initializeDisk(&myDisk);

    // Display the disk status
    displayDiskStatus(&myDisk);

    return 0;
}
