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
    disk->freeBlocks = DISK_SIZE; // Initially, all blocks are free

    for (int i = 0; i < DISK_SIZE; i++) {
        disk->blocks[i].isFree = 1;
        memset(disk->blocks[i].data, 0, BLOCK_SIZE); // Clear block data!!
    }
           //note to selF :come back
    printf("Disk initialized with %d blocks, each of size %d bytes.\n", DISK_SIZE, BLOCK_SIZE);
}

// Function to clear all data in the disk (secondary memory)
void clearDisk(Disk *disk) {
    for (int i = 0; i < DISK_SIZE; i++) {
        disk->blocks[i].isFree = 1; // Mark all blocks as free
        memset(disk->blocks[i].data, 0, BLOCK_SIZE); // Clear all block data
    }
    disk->freeBlocks = DISK_SIZE; // Reset the free block counter

    printf("Disk has been cleared. All blocks are now free.\n");
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

    // Clear the disk
    clearDisk(&myDisk);

    // Display the disk status after clearing
    displayDiskStatus(&myDisk);

    return 0;
}
