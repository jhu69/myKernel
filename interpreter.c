//
//  interpreter.c
//  Kernel_Process
//
//  Created by Jack Hu on 2019-02-22.
//  Copyright © 2019 Jack Hu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "ram.h"

// This function will interpret the commands & argument when passed and will modify shell memory when appropriate
int interpreter(char *words[]) {
    
    // Instantiating error code
    int errorCode = 0;
    
    // User is asking to execute one of these commands 
    if (strcmp(words[0], "run") == 0) {
        errorCode = script(words);
    } else if (strcmp(words[0], "exec") == 0) {
        errorCode = exec(words);
    } else if (strcmp(words[0], "printPCB") == 0) {
        errorCode = printProcessControlBlock();
    } else if (strcmp(words[0], "quit") == 0) {
        errorCode = quit();
    } else if (strcmp(words[0], "help") == 0) {
        errorCode = help();
    } else if (strcmp(words[0], "set") == 0) {
        errorCode = set(words);
    } else if (strcmp(words[0], "print") == 0) {
        errorCode = print(words);
    } else {
        printf("\nUnkown Command!\n\n");
        errorCode = 0;
    }
    // returns a certain error code according to each individual command
    return errorCode;
}

// Displays all the commands this OS supports
int help() {
    printf("\n");
    printf("These are the available commands below.\n");
    printf("Command format: COMMAND ARGUMENTS \n");
    printf("1) quit: Exits/terminates the shell \n");
    printf("2) help: Display all the commands \n");
    printf("3) set: Assigns a value to shell memory \n");
    printf("4) print: Prints the STRING assigned to VAR  \n");
    printf("5) run: Executes the file SCRIPT.TXT or any other txt file \n");
    printf("6) exec: Executes concurret programs \n");
    printf("7) printPCB: Testing dequeue function in pcb.c \n");
    printf("\n");
    return 0; // error code for no error default code
}

// Quits shell with text bye and returns error code -1 for the infinite while loop to terminate
int quit() {
    printf("\nBye!\n\n");
    return (-1);
}

// checks to see if VAR already exists, if it exits then STRING overwrites the previous value assigned to VAR in shell
// memory, if VAR does not exist then creates a new variable and assign it the input STRING
int set(char *words[]) {
    setVarString(words[1], words[2]);
    printf("\nSuccess!\n\n");
    return 0;
}

// prints string assinged to VAR, checks to see if it exists, if it does then prints it, otherwise prints
// "Variable does not exist"
int print(char *words[]) {
    printValue(words[1]);
    return 0;
}

// user is asking to execute the run command which reads line by line in a text file of all the
// commands, once it gets to a newline then script closes the text file and displays the command
// prompt once more
// in the case while its reading the command and it encounters a quit command then the file closes and commandline prompt terminates
int script(char *words[]) {
    
    // to get rid of the newline char at the end of the script name
    //char *tmp = strtok(words[1], "\n");
    int errCode = 0;
    char scriptInput[1000];
    
    // in the case that the script text file does not exist, just exits the program
    FILE *file = fopen(words[1], "r");
    if (file == NULL) {
        printf("\nScript not found!\n\n");
        return errCode;
    }
    
    // reading each line of the opened text file and sending it to interpreter as commands for shell to do something
    //fgets(scriptInput, 999, file);
    while (fgets(scriptInput, sizeof(scriptInput), file) != NULL) {
        errCode = parse(scriptInput);
        if (errCode == -1) {
            fclose(file);
            return errCode;
        }
    }
    fclose(file);
    return errCode;
}

// exec command/function runs concurrent scripts and checks to see if script with
// same name has been initiated, if it has then throw error
int exec(char *words[]) {
    
    // To count how many arguments exec executes with
    // It could have "exec arg1 arg2 arg3" or "exec arg1 arg2" or "exec arg1"
    int numArgs = 0;
    int errCode = 0;
    for (int i = 1; i<=3; i++) {
        if(words[i] != NULL) {
            numArgs++;
        }
    }
    // To switch between diff versions of exec depending on how many args user wants
    printf("\nNumber of arguments: %d\n", numArgs);
    switch (numArgs) {
        case 1:
            printf("exec with 1 prog!\n\n");
            FILE *prog1 = fopen(words[1], "r");
            if (prog1 == NULL) {
                printf("\nScript not found!\n\n");
                fclose(prog1);
                return errCode;
            } else {
                myInit(prog1, words[1]);
                scheduler();
                clean();
                fclose(prog1);
                return errCode;
            }
        case 2:
            printf("exec with 2 prog!\n\n");
            if (strcmp(words[1], words[2]) == 0) {
                printf("\nError: Script %s already loaded.\n\n", words[1]);
                FILE *prog2 = fopen(words[1], "r");
                if (prog2 == NULL) {
                    printf("\nScript not found!\n\n");
                    fclose(prog2);
                    return errCode;
                } else {
                    myInit(prog2, words[1]);
                    scheduler();
                    clean();
                    fclose(prog2);
                    return errCode;
                }
            } else {
                FILE *prog3 = fopen(words[1], "r");
                FILE *prog4 = fopen(words[2], "r");
                
                if (prog3 == NULL && prog4 == NULL) {
                    printf("\nScript not found!\n\n");
                    fclose(prog3);
                    fclose(prog4);
                    return errCode;
                } else if( prog3 != NULL && prog4 == NULL) {
                    myInit(prog3, words[1]);
                    printf("\nSecond script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog3);
                    fclose(prog4);
                    return errCode;
                } else if (prog3 == NULL && prog4 != NULL) {
                    myInit(prog4, words[2]);
                    printf("\nFirst script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog4);
                    fclose(prog3);
                    return errCode;
                } else {
                    myInit(prog3, words[1]);
                    myInit(prog4, words[2]);
                    scheduler();
                    clean();
                    fclose(prog3);
                    fclose(prog4);
                    return errCode;
                }
            }
        case 3:
            printf("exec with 3 prog!\n\n");
            if (strcmp(words[1], words[2]) == 0) {
                if (strcmp(words[2], words[3]) == 0) {
                    if(strcmp(words[1], words[3]) == 0) {
                        
                        printf("\nError: script %s already loaded.\n\n", words[1]);
                        FILE *progD = fopen(words[1], "r");
                        if (progD == NULL) {
                            printf("\nScript not found!\n\n");
                            fclose(progD);
                            return errCode;
                        } else {
                            myInit(progD, words[1]);
                            scheduler();
                            clean();
                            fclose(progD);
                            return errCode;
                        }
                    }
                }
                printf("\nError: Script %s already loaded.\n\n", words[2]);
                FILE *prog5 = fopen(words[1], "r");
                FILE *prog6 = fopen(words[3], "r");
                if (prog5 == NULL && prog6 == NULL) {
                    printf("\nScript not found!\n\n");
                    fclose(prog5);
                    fclose(prog6);
                    return errCode;
                } else if( prog5 != NULL && prog6 == NULL) {
                    myInit(prog5, words[1]);
                    printf("\nThird script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog5);
                    fclose(prog6);
                    return errCode;
                } else if(prog5 == NULL && prog6 != NULL) {
                    myInit(prog6, words[3]);
                    printf("\nFirst script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog6);
                    fclose(prog5);
                    return errCode;
                } else {
                    myInit(prog5, words[1]);
                    myInit(prog6, words[3]);
                    scheduler();
                    clean();
                    fclose(prog5);
                    fclose(prog6);
                    return errCode;
                }
            } else if (strcmp(words[2], words[3]) == 0) {
                printf("\nError: Script %s already loaded.\n\n", words[3]);
                FILE *prog7 = fopen(words[1], "r");
                FILE *prog8 = fopen(words[2], "r");
                if (prog7 == NULL && prog8 == NULL) {
                    printf("\nScript not found!\n\n");
                    fclose(prog7);
                    fclose(prog8);
                    return errCode;
                } else if( prog7 != NULL && prog8 == NULL) {
                    myInit(prog7, words[1]);
                    printf("\nSecond script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog7);
                    fclose(prog8);
                    return errCode;
                } else if (prog7 == NULL && prog8 != NULL) {
                    myInit(prog8, words[2]);
                    printf("\nFirst script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog8);
                    fclose(prog7);
                    return errCode;
                } else {
                    myInit(prog7, words[1]);
                    myInit(prog8, words[2]);
                    scheduler();
                    clean();
                    fclose(prog7);
                    fclose(prog8);
                    return errCode;
                }
            } else if(strcmp(words[1], words[3]) == 0) {
                printf("\nError: Script %s already loaded.\n\n", words[1]);
                FILE *prog9 = fopen(words[1], "r");
                FILE *prog10 = fopen(words[2], "r");
                
                if (prog9 == NULL && prog10 == NULL) {
                    printf("\nScripts not found!\n\n");
                    fclose(prog9);
                    fclose(prog10);
                    return errCode;
                } else if( prog9 != NULL && prog10 == NULL) {
                    myInit(prog9, words[1]);
                    printf("\nSecond script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog9);
                    fclose(prog10);
                    return errCode;
                } else if (prog9 == NULL && prog10 != NULL) {
                    myInit(prog10, words[2]);
                    printf("\nFirst script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(prog9);
                    fclose(prog10);
                    return errCode;
                } else {
                    myInit(prog9, words[1]);
                    myInit(prog10, words[2]);
                    scheduler();
                    clean();
                    fclose(prog9);
                    fclose(prog10);
                    return errCode;
                }
            } else {
                FILE *progA = fopen(words[1], "r");
                FILE *progB = fopen(words[2], "r");
                FILE *progC = fopen(words[3], "r");
                
                if (progA == NULL && progB == NULL && progC == NULL) {
                    printf("\nScript not found!\n\n");
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                } else if( progA != NULL && progB == NULL && progC == NULL ) {
                    myInit(progA, words[1]);
                    printf("\nSecond & thrid scripts not found!\n\n");
                    scheduler();
                    clean();
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                } else if (progA != NULL && progB != NULL && progC == NULL) {
                    myInit(progA, words[1]);
                    myInit(progB, words[2]);
                    printf("\nThrid script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                } else if (progA != NULL && progB != NULL && progC != NULL) {
                    myInit(progA, words[1]);
                    myInit(progB, words[2]);
                    myInit(progC, words[3]);
                    scheduler();
                    clean();
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                } else if (progA != NULL && progB == NULL && progC != NULL) {
                    myInit(progA, words[1]);
                    myInit(progC, words[3]);
                    printf("\nSecond script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                } else {
                    myInit(progB, words[2]);
                    myInit(progC, words[3]);
                    printf("\nFirst script not found!\n\n");
                    scheduler();
                    clean();
                    fclose(progA);
                    fclose(progB);
                    fclose(progC);
                    return errCode;
                }
            }
    }
    printf("\nError! Did not find any programs!\n");
    return errCode;
}

// Added function during the ready queue testing phase 
// Testing dequeue from pcb.c
int printProcessControlBlock() {
    int errorCode = 0;
    printPCB();
    return errorCode;
}




