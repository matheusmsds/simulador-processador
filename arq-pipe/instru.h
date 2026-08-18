#ifndef INSTRU_H
#define INSTRU_H


#define PIPE_ADD             0
#define PIPE_SUB             1
#define PIPE_MUL             2
#define PIPE_DIV             3

#define PIPE_CMP_EQUAL       4
#define PIPE_CMP_NEQ         5
#define PIPE_CMP_LESS        6
#define PIPE_CMP_GREATER     7
#define PIPE_CMP_LESS_EQ     8
#define PIPE_CMP_GREATER_EQ  9
#define PIPE_AND             10
#define PIPE_OR              11
#define PIPE_XOR             12
#define PIPE_SHL             13
#define PIPE_SHR             14

#define PIPE_LOAD            15
#define PIPE_STORE           16

#define PIPE_JUMP            0
#define PIPE_JUMP_COND       1
#define PIPE_MOV             3
#define PIPE_SYSCALL         63


#endif