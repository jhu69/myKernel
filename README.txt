OS required: mimi.cs.mcgill.ca

What mykernel is capable of: 

1) All of the commands from assignment 1 are still fully functional
2) exec command works for all input cases even when combined with null program inputs 
(eg. exec prog1.txt jack prog2.txt, etc etc)
3) When testing mykernel with ./mykernel < testfile.txt, if the last command is quit then it quits out of mykernel. Or else the command prompt is shown to user again. 
4) For the above 3) after the testfile.txt gets put in when the flow of execution ends, it some how shows the $ twice but the execution does stop which is nice haha 
5) I've added extra fields in pcb struct and modified myInit() to take in a file pointer and a string name to help determine which pcb (after creation) is being enqueued and dequeued during run time. They should all execute in the correct manner.
6) Also just for simplicity, please end my program with a quit command that way makes life easier for everyone :)
7) I have no idea why when i do ./mykernel < testfile.txt on my terminal on my mac there are no segmentation faults but when i do the exact same on mimi sometimes it appear but sometimes it doesn't 

Thanks for grading! This assignment was a lot of fun! :) 
