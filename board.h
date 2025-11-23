#pragma once
#ifndef ASS4_BOARD_H
#define ASS4_BOARD_H

#include "util.h"

/*
 * Initialization of global variables
 */

filedescriptors fds = {-1, -1, -1, -1};
mappings mmaps = {NULL, NULL, NULL, NULL};
pid_t process_id = -1;

/*
 * Default functions for board.c
 */

int startboard();
void initProcess();
void initSharedMemoriesBoard();
void initMemoryMappingsBoard();
void closeMmapingsBoard();

char* cmdHandler(shmrequest* request);

/*
 * Various helpers for board.c
 * Use them!
 */

bool isCommand(const char* cmd, const shmrequest* request);
char getAnswerArgument(const shmrequest* request);
void setupQuizstate(shmquizsstate* quizstate);
char* generateQuestionText(const shmquizsstate* quizstate);
char* generateResultsText(const shmquizsstate* quizstate);
void answerCurrentQuestion(shmquizsstate* quizstate, char answer);
void skipCurrentQuestion(shmquizsstate* quizstate);

/*
 * Question Pool
 */

typedef struct {
  char* question_;
  char* answers_[4];
  char correct_answer_;
} question_t;

// clang-format off
question_t QUESTION_POOL[] = {
  {
    .question_ = "What is the primary function of an operating system?",
    .answers_ = {"Manage hardware resources", "Compile code", "Render Webpages", "Create databases"},
    .correct_answer_ = 'a'
  }, {
    .question_ = "Which of the following is a low-level programming language?",
    .answers_ = {"Python", "Java", "C#", "Assembly"},
    .correct_answer_ = 'd'
  }, {
    .question_ = "In computer memory, what does RAM stand for?",
    .answers_ = {"Read-Access Memory", "Random-Access Memory", "Run-All Memory", "Reserve-Area Memory"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "Which part of the CPU performs arithmetic and logic operations?",
    .answers_ = {"Control Unit", "Arithmetic Logic Unit (ALU)", "Register", "Cache"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "What does the term 'booting' refer to?",
    .answers_ = {"Loading the operating system", "Installing software", "Shutting down the system", "Updating hardware drivers"},
    .correct_answer_ = 'a'
  }, {
    .question_ = "Which of the following is used to directly communicate with hardware?",
    .answers_ = {"Compiler", "Interpreter", "Driver", "Debugger"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "Which register typically stores the address of the next instruction to be executed?",
    .answers_ = {"Instruction Register", "Program Counter", "Stack Pointer", "Base Register"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "What is the primary role of a stack in low-level programming?",
    .answers_ = {"To store variables", "To handle function calls and return addresses", "To manage memory allocation", "To execute loops"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "Which of the following is NOT an essential part of a process control block (PCB)?",
    .answers_ = {"Process ID", "Program Counter", "CPU Registers", "Program Source Code"},
    .correct_answer_ = 'd'
  }, {
    .question_ = "What does a system call allow a program to do?",
    .answers_ = {"Request services from the kernel", "Access data in the CPU cache", "Control hardware directly", "Write data to registers"},
    .correct_answer_ = 'a'
  }, {
    .question_ = "What is the function of a memory management unit (MMU)?",
    .answers_ = {"Execute programs", "Control peripherals", "Manage memory addresses", "Coordinate process communication"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "Which type of memory is typically used for the stack in low-level programming?",
    .answers_ = {"Heap", "Cache", "RAM", "ROM"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "What is the purpose of an interrupt in a CPU?",
    .answers_ = {"To stop the program", "To alert the CPU of an event", "To start a program", "To switch between processes"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "What does the term 'context switch' refer to in an operating system?",
    .answers_ = {"Starting a new program", "Switching between two processes", "Interrupt handling", "Memory management"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "What is the function of a linker in software development?",
    .answers_ = {"Compile source code", "Resolve addresses and link object files", "Execute programs", "Debug code"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "Which of the following is a common file extension for object files?",
    .answers_ = {".exe", ".c", ".o", ".txt"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "Which instruction is used to push data onto the stack in x86-64 assembly language?",
    .answers_ = {"POP", "JMP", "PUSH", "MOV"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "What is a characteristic of a critical section in concurrent programming?",
    .answers_ = {"It can be accessed by multiple threads simultaneously", "It must be executed by one thread at a time", "It is code that handles errors", "It is used for memory allocation"},
    .correct_answer_ = 'b'
  }, {
    .question_ = "Which of the following is an example of a low-level hardware interface?",
    .answers_ = {"GUI", "API", "BIOS", "IDE"},
    .correct_answer_ = 'c'
  }, {
    .question_ = "What is the purpose of a semaphore in operating systems?",
    .answers_ = {"Synchronize processes", "Compile code", "Manage memory", "Handle interrupts"},
    .correct_answer_ = 'a'
  }
};
// clang-format on

#define NUM_QUESTIONS_IN_POOL (sizeof(QUESTION_POOL) / sizeof(question_t))

#endif // ASS4_BOARD_H
