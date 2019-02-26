//
//  kernel.c
//  Kernel_Process
//
//  Created by Jack Hu on 2019-02-22.
//  Copyright © 2019 Jack Hu. All rights reserved.
//

#include "kernel.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"
#include "shellmemory.h"


// Kernel's main function, this is the root function of all process executions
int main(int argc, const char * argv[]) {

    // Kernel's initialization of Ready, Ram, and ShellMemory, etc
    initReadyQueue();
    initRAM();
    initShellMemory();
    // Loads kernel UI shell after all of kernel's initialization passes
    printf("\nKernel 1.0 loaded!\n");
    printf("Welcome to Jack Hu's Shell!\n");
    printf("Shell version 2.0 Updated February 2019\n\n");
    displayShellPromptAndPassCommand();
    return 0;
}

// Adds PCB to the end of Ready Queue
void addToReady(struct PCB* pcb) {
    enqueue(pcb);
}

// Testing dequeue from pcb.c, not part of this assignment
void printPCB() {
    dequeue();
}

// Kernel's initialization function to load file ptr into ram, malloc a PCB, and set file ptr to PCB's
// Program counter, load PCB into Ready Queue
// Added an extra field to makePCB to keep track of which prog I'm running 
void myInit(FILE *p, char *pcbName) {
    addToRAM(p);
    struct PCB* newPCB = makePCB(p, pcbName);
    addToReady(newPCB);
}

// Kernel's command to run all active processes in ready queue
void scheduler() {
    
    bool done = false;
    struct CPU* myCPU = makeCPU();
    struct PCB* retrievedPCB;
    while(!done) {
        if ( (myCPU->IP == NULL) || (myCPU->quanta == 2) ) {
            retrievedPCB = dequeue();
            myCPU->IP = retrievedPCB->programCounter;
            myCPU = run(myCPU);
            retrievedPCB->programCounter = myCPU->IP;
            if(retrievedPCB->programCounter == NULL) {
                printf("Program: %s terminated\n\n", retrievedPCB->name);
                free(retrievedPCB);
                if (isReadyQueueEmpty() == true) {
                    done = true;
                }
            } else {
                enqueue(retrievedPCB);
            }
            
        }
    }
    free(myCPU);
}



