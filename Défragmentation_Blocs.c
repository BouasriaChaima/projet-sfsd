#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 100
#define BLOCK_SIZE 512

// Block structure
typedef struct {
    int isFree;            // 1 if block is free, 0 if used
    char data[BLOCK_SIZE]; // Content of the block
} Block;

// Disk structure
typedef struct {
    Block blocks[DISK_SIZE]; // Array of blocks
    int freeBlocks;          // Number of free blocks
} Disk;

// Function to initialize the disk
void initializeDisk(Disk *disk) {
    disk->freeBlocks = DISK_SIZE;

    for (int i = 0; i < DISK_SIZE; i++) {
        disk->blocks[i].isFree = 1;
        memset(disk->blocks[i].data, 0, BLOCK_SIZE);
    }

    printf("Disk initialized with %d blocks.\n", DISK_SIZE);
}

// Function to simulate adding data to a block
void addDataToBlock(Disk *disk, int blockIndex, const char *data) {
    if (blockIndex >= 0 && blockIndex < DISK_SIZE && disk->blocks[blockIndex].isFree) {
        disk->blocks[blockIndex].isFree = 0;
        strncpy(disk->blocks[blockIndex].data, data, BLOCK_SIZE - 1);
        disk->blocks[blockIndex].data[BLOCK_SIZE - 1] = '\0'; // Null-terminate
        disk->freeBlocks--;
    } else {
        printf("Error: Cannot add data to block %d.\n", blockIndex);
    }
}

// Function to defragment the disk
void defragmentDisk(Disk *disk) {
    int writeIndex = 0; // Tracks where to move used blocks

    for (int readIndex = 0; readIndex < DISK_SIZE; readIndex++) {
        if (!disk->blocks[readIndex].isFree) {
            // Move the block if needed
            if (readIndex != writeIndex) {
                // Copy data to the write position
                disk->blocks[writeIndex] = disk->blocks[readIndex];

                // Mark the old position as free
                disk->blocks[readIndex].isFree = 1;
                memset(disk->blocks[readIndex].data, 0, BLOCK_SIZE);
            }

            writeIndex++;
        }
    }

    printf("Disk defragmentation complete. All used blocks are now contiguous.\n");
}

// Function to display the status of the disk
void displayDiskStatus(Disk *disk) {
    printf("Disk Status:\n");
    printf("Free Blocks: %d\n", disk->freeBlocks);
    printf("Blocks:\n");

    for (int i = 0; i < DISK_SIZE; i++) {
        printf("Block %d: %s\n", i, disk->blocks[i].isFree ? "Free" : "Used");
    }
}

int main() {
    Disk myDisk;

    // Initialize the disk
    initializeDisk(&myDisk);

    // on ajoute ici some data to simulate usage
    addDataToBlock(&myDisk, 0, "File 1");
    addDataToBlock(&myDisk, 2, "File 2");
    addDataToBlock(&myDisk, 5, "File 3");

    printf("\nBefore Defragmentation:\n");
    displayDiskStatus(&myDisk);

    // Defragment the disk
    defragmentDisk(&myDisk);

    printf("\nAfter Defragmentation:\n");
    displayDiskStatus(&myDisk);

    return 0;
}
