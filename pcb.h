//
//  pcb.h
//  Kernel_Process
//
//  Created by Jack Hu on 2019-02-22.
//  Copyright © 2019 Jack Hu. All rights reserved.
//

#ifndef pcb_h
#define pcb_h

#include <stdio.h>
#include <stdbool.h>


// Instantiating PCB "object" & LinkedListNode of type PCB, stores a file ptr and name
struct PCB {
    FILE *programCounter;
    struct PCB *next;
    char *name;
};


// Method instantiations that are "public"
void initReadyQueue(void);
struct PCB* makePCB(FILE *p, char *pcbName);
void enqueue(struct PCB* pcb);
struct PCB* dequeue(void);
bool isReadyQueueEmpty(void);

#endif /* pcb_h */
