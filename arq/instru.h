#ifndef INSTRU_H
#define INSTRU_H


#define MONO_ADD             0
#define MONO_SUB             1
#define MONO_MUL             2
#define MONO_DIV             3

#define MONO_CMP_EQUAL       4
#define MONO_CMP_NEQ         5
#define MONO_CMP_LESS        6
#define MONO_CMP_GREATER     7
#define MONO_CMP_LESS_EQ     8
#define MONO_CMP_GREATER_EQ  9
#define MONO_AND             10
#define MONO_OR              11
#define MONO_XOR             12
#define MONO_SHL             13
#define MONO_SHR             14

#define MONO_LOAD            15
#define MONO_STORE           16

#define MONO_JUMP            0
#define MONO_JUMP_COND       1
#define MONO_MOV             3
#define MONO_SYSCALL         63


#endif