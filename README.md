# CPSC 351 Assignment 2: Process Synchronization

**LANGUAGE:** C++

## Overview

This github repository serves as a method for creating a teaching assistant program. Below are background information regarding this assignment:

A university computer science department has a teaching assistant (TA) who helps
undergraduate students with their programming assignments during regular office hours. The
TA’s office is rather small and has room for only one desk with a chair and computer. There are
three chairs in the hallway outside the office where students can sit and wait if the TA is
currently helping another student. When there are no students who need help during office hours,
the TA sits at the desk and takes a nap.

If a student arrives during office hours and finds the TA sleeping, the student must awaken the
TA to ask for help. If a student arrives and finds the TA currently helping another student, the student sits on one of the chair in the hallway and waits. If no chairs are available, the student will come back at a later time.

## Student Information

### Adrian Diaz

-   **Section:** 06
-   **Email Address:** adiaz20@csu.fulllerton.edu

### Eduardo Guerra

-   **Section:** 06
-   **Email Address:** edguerra@csu.fulllerton.edu

### Vincent Polanco

-   **Section:** 06
-   **Email Address:** vincent.polanco@csu.fulllerton.edu

## How to execute this program

This program can be executed using any C++ compiler, particularly g++. To execute within the terminal, please use the command: `g++ processsync.cpp`.

Then, run the executional file using the command: `./a.out`. An output should display within your desired Linux terminal.

## File Guide

-   `processsync.cpp:` Main program file of the project
-   `README.md`: Initialization file
-   `Design of Sleeping Teaching Assistant:` Design file with necessary design patterns
