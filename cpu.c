//
//  cpu.c
//  Kernel_Process
//
//  Created by Jack Hu on 2019-02-22.
//  Copyright © 2019 Jack Hu. All rights reserved.
//

#include "cpu.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Mallocs a CPU to run programs
struct CPU* makeCPU() {
    struct CPU *cpu = (struct CPU*) malloc(sizeof(struct CPU));
    cpu->quanta = 2;
    return cpu;
}

// Runs my CPU for a quanta = 2 (execute 2 lines of instructions) 
struct CPU* run(struct CPU* cpu) {
    while(cpu->quanta != 0) {
        if (fgets(cpu->instructionReg, sizeof(cpu->instructionReg) , cpu->IP) == NULL) {
            cpu->quanta += 2;
            cpu->IP = NULL;
            return cpu;
        }
        parse(cpu->instructionReg);
        memset(cpu->instructionReg,'\0', 1000);
        cpu->quanta--;
    }
    memset(cpu->instructionReg,'\0', 1000);
    cpu->quanta+=2;
    return cpu;
}



