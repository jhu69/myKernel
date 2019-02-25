//
//  ram.c
//  Kernel_Process
//
//  Created by Jack Hu on 2019-02-22.
//  Copyright © 2019 Jack Hu. All rights reserved.
//

#include "ram.h"
#include <string.h>

// Simulates OS memory to run processes
FILE *ram[10];
// Keeps track of which ram cell index programs are put into, i increases after each add to ram
int i;

// Initializes ram
void initRAM(){
    // Simulates OS memory to run processes
    FILE *ram[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    // Keeps track of which ram cell index programs are put into, i increases after each add to ram
    int i = 0;
}

// Adds the fopened file ptr to the ram data structure which holds an array of file ptrs
void addToRAM(FILE *ptr) {
    
    int hashedIndex;
    hashedIndex = (i % 10);
    ram[hashedIndex] = ptr;
    printf("File stored at %d th position of ram!\n", i);
    i++;
}

// Cleans up ram and resets the ram add index after all programs have terminated
void clean() {
    i = 0;
    memset(ram,'\0',10);
}









