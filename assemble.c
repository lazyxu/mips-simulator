//
//  assemble.c
//  test
//
//  Created by Meteor on 2016/5/11.
//  Copyright © 2016年 Meteor. All rights reserved.
//

#include "assemble.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char immediateBits[17];
extern int isDebug;
int lable_address[30];
char lable[30][20];
int lable_index=0;
int Strcmp(char *s1, char *s2)
{
    while (*s1!=0||*s2!=0) {
        if (*s1!=*s2) {
            return 0;
        }
        s1++;
        s2++;
    }
    return 1;
}

void immediateToBit( char* str, int length)
{
    int i, n1=0, n2=0, flag=0;
    for (i=0; i<length; i++) {
        if ( str[i] == '+' ) {
            flag = 1;
            break;
        }
        if ( str[i] == '-' ) {
            flag = 2;
            break;
        }
        if ( str[i] == '*' ) {
            flag = 3;
            break;
        }
        n1 *= 10;
        n1 += str[i]-'0';
        
    }
    if ( flag != 0 ) {
        for ( i++ ; i<length; i++) {
            n2 *= 10;
            n2 += str[i]-'0';
        }
    }
    switch (flag) {
        case 1:
            n1 = n1 + n2;
            break;
        case 2:
            n1 = n1 - n2;
            break;
        case 3:
            n1 = n1 * n2;
            break;
        default:
            break;
    }
    if ( n1 == 0 ) {
        strcpy(immediateBits, "0000000000000000");
        return ;
    }
    i=15;
    while ( n1 != 1 ) {
        immediateBits[i] = n1 % 2 + '0';
        n1 = n1 / 2;
        i--;
    }
    immediateBits[i] = '1';
    for ( i-- ; i >= 0; i--) {
        immediateBits[i] = '0';
    }
}

unsigned int strToInt(char *str)
{
    int i;
    int n=0;
    if (str[0]=='0'&&str[1]=='x') {// 16进制
        for (i=2; str[i]!='\0'; i++) {
            n = n * 16;
            if ( str[i]>='0' && str[i]<='9' )
                n += str[i] - '0';
            else if ( str[i]>='a' && str[i]<='f' )
                n += str[i] - 'a' + 10;
            else if ( str[i]>='A' && str[i]<='F' )
                n += str[i] - 'A' + 10;
            else
                return 0xffffffff;
        }
    }
    else if (str[0]>='0'&&str[0]<='9'){ // 10进制
        for (i=0; str[i]!='\0'; i++) {
            n = n * 10;
            if ( str[i]>='0' && str[i]<='9' )
                n += str[i] - '0';
            else
                return 0xffffffff;
        }
    }
    else if (str[0]=='-'){ // 负数, 10进制
        for (i=1; str[i]!='\0'; i++) {
            n = n * 10;
            if ( str[i]>='0' && str[i]<='9' )
                n += str[i] - '0';
            else
                return 0xffffffff;
        }
        n=-n;
    }
    else {
        for (i=0;i<lable_index;i++) {
            if (strcmp(str, lable[i])==0)
                return lable_address[i];
        }
        if(isDebug)  printf("target not found");
    }
    return n;
}

char *sub_string_assemble(char* string, int start, int end)
{
    char *substr;
    int i;
    substr = malloc(sizeof(char)*(end-start+2));
    for (i=start; i<=end; i++) {
        substr[i-start] = string[i];
    }
    substr[i-start] = '\0';
    return substr;
}

int get_string_assemble(char* getstr, char* string, int n, int max_len, char *skip_char)
{
    //char *getstr;
    //getstr = malloc(sizeof(char)*max_len);
    int i, j, index=0, flag;
    
    while ( string[n+index] != '\0' ) {
        flag = 0;
        j = 0;
        while ( skip_char[j] != '\0') {
            if( string[n+index] == skip_char[j] ) {
                index++;
                flag = 1;
                break;
            }
            j++;
        }
        if ( flag != 1 ) {
            break;
        }
    }
    
    for (i=0; i<max_len; i++) {
        j = 0;
        while ( skip_char[j] != '\0') {
            if( string[n+index] == skip_char[j] ) {// 过滤字符
                getstr[i] = '\0';
                return n+index;
            }
            j++;
        }
        if( string[n+index] == '\0' ) {
            getstr[i] = '\0';
            return n+index;
        }
        getstr[i] = string[n+index];
        index++;
    }
    return n+index;
}

unsigned int registerToNumber(char *str)
{
    unsigned int reg = 0;
    if (Strcmp(str, "$zero")||Strcmp(str, "$0")) return reg;
    reg++;
    if (Strcmp(str, "$at")||Strcmp(str, "$1")) return reg;
    reg++;
    if (Strcmp(str, "$v0")||Strcmp(str, "$2")) return reg;
    reg++;
    if (Strcmp(str, "$v1")||Strcmp(str, "$3")) return reg;
    reg++;
    if (Strcmp(str, "$a0")||Strcmp(str, "$4")) return reg;
    reg++;
    if (Strcmp(str, "$a1")||Strcmp(str, "$5")) return reg;
    reg++;
    if (Strcmp(str, "$a2")||Strcmp(str, "$6")) return reg;
    reg++;
    if (Strcmp(str, "$a3")||Strcmp(str, "$7")) return reg;
    reg++;
    if (Strcmp(str, "$t0")||Strcmp(str, "$8")) return reg;
    reg++;
    if (Strcmp(str, "$t1")||Strcmp(str, "$9")) return reg;
    reg++;
    if (Strcmp(str, "$t2")||Strcmp(str, "$10")) return reg;
    reg++;
    if (Strcmp(str, "$t3")||Strcmp(str, "$11")) return reg;
    reg++;
    if (Strcmp(str, "$t4")||Strcmp(str, "$12")) return reg;
    reg++;
    if (Strcmp(str, "$t5")||Strcmp(str, "$13")) return reg;
    reg++;
    if (Strcmp(str, "$t6")||Strcmp(str, "$14")) return reg;
    reg++;
    if (Strcmp(str, "$t7")||Strcmp(str, "$15")) return reg;
    reg++;
    if (Strcmp(str, "$s0")||Strcmp(str, "$16")) return reg;
    reg++;
    if (Strcmp(str, "$s1")||Strcmp(str, "$17")) return reg;
    reg++;
    if (Strcmp(str, "$s2")||Strcmp(str, "$18")) return reg;
    reg++;
    if (Strcmp(str, "$s3")||Strcmp(str, "$19")) return reg;
    reg++;
    if (Strcmp(str, "$s4")||Strcmp(str, "$20")) return reg;
    reg++;
    if (Strcmp(str, "$s5")||Strcmp(str, "$21")) return reg;
    reg++;
    if (Strcmp(str, "$s6")||Strcmp(str, "$22")) return reg;
    reg++;
    if (Strcmp(str, "$s7")||Strcmp(str, "$23")) return reg;
    reg++;
    if (Strcmp(str, "$t8")||Strcmp(str, "$24")) return reg;
    reg++;
    if (Strcmp(str, "$t9")||Strcmp(str, "$25")) return reg;
    reg++;
    if (Strcmp(str, "$k0")||Strcmp(str, "$26")) return reg;
    reg++;
    if (Strcmp(str, "$k1")||Strcmp(str, "$27")) return reg;
    reg++;
    if (Strcmp(str, "$gp")||Strcmp(str, "$28")) return reg;
    reg++;
    if (Strcmp(str, "$sp")||Strcmp(str, "$29")) return reg;
    reg++;
    if (Strcmp(str, "$fp")||Strcmp(str, "$30")) return reg;
    reg++;
    if (Strcmp(str, "$ra")||Strcmp(str, "$31")) return reg;
    return 0xffffffff;
}

int Assemble(char *command, unsigned int current_pc)
{
    char operator[8],s_reg[10];
    int i=0;
    unsigned int b_opcode, b_rd, b_rs, b_rt, b_sa, b_func = 0, b_immediate, b_target, b_format, b_ft, b_fs, b_fd;
    if ( command[0]==0 )
        return 0x00000000;
    i = get_string_assemble(&operator[0],command, i, sizeof(operator), " ");
    if(isDebug)  printf("%s  ", operator);
    if ( strcmp(operator, "q")==0 )
        return 0xffffff00;
    if ( operator[strlen(operator)-1]==':' ) {
        return 0xfffffff0;
    }
    //R-Type Instructions
    b_opcode = 0b000000;
    b_rd = 0;
    b_rs = 0;
    b_rt = 0;
    b_sa = 0;
    if ( strcmp(operator, "add")==0 ) {
        b_func = 0b100000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "addu")==0 ) {
        b_func = 0b100001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "and")==0 ) {
        b_func = 0b100100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "break")==0 ) {
        b_func = 0b001101;
    }
    else if ( strcmp(operator, "div")==0 ) {
        b_func = 0b011010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "divu")==0 ) {
        b_func = 0b011011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "jalr")==0 ) {
        b_func = 0b001001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "jr")==0 ) {
        b_func = 0b001000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "mfhi")==0 ) {
        b_func = 0b010000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "mflo")==0 ) {
        b_func = 0b010010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "mthi")==0 ) {
        b_func = 0b010001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "mtlo")==0 ) {
        b_func = 0b010011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "mult")==0 ) {
        b_func = 0b011000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "multu")==0 ) {
        b_func = 0b011001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "nor")==0 ) {
        b_func = 0b100111;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "or")==0 ) {
        b_func = 0b100101;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "sll")==0 ) {
        b_func = 0b000000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_sa = strToInt(s_reg);
    }
    else if ( strcmp(operator, "sllv")==0 ) {
        b_func = 0b000100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "slt")==0 ) {
        b_func = 0b101010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "sltu")==0 ) {
        b_func = 0b101011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "sra")==0 ) {
        b_func = 0b000011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_sa = strToInt(s_reg);
    }
    else if ( strcmp(operator, "srav")==0 ) {
        b_func = 0b000111;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "srl")==0 ) {
        b_func = 0b000010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_sa = strToInt(s_reg);
    }
    else if ( strcmp(operator, "srlv")==0 ) {
        b_func = 0b000110;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "sub")==0 ) {
        b_func = 0b100010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "subu")==0 ) {
        b_func = 0b100011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "syscall")==0 ) {
        b_func = 0b001100;
    }
    else if ( strcmp(operator, "xor")==0 ) {
        b_func = 0b100110;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
    }
    else
        b_func = 0xffffffff;
    if ( (b_rd|b_rs|b_rt|b_sa)==0xffffffff )
        return 0xffffffff;
    if(isDebug)  printf("R?  %x %x %x %x %x %x\n", b_opcode, b_rd, b_rs, b_rt, b_sa, b_func);
    if ( b_func != 0xffffffff )
        return (b_opcode<<26) | (b_rs<<21) | (b_rt<<16) | (b_rd<<11) | (b_sa<<6) | b_func;
    
    //I-Type Instructions
    b_rt = 0;
    b_rs = 0;
    b_immediate = 0;
    if ( strcmp(operator, "addi")==0 ) {
        b_opcode = 0b001000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "addiu")==0 ) {
        b_opcode = 0b001001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "andi")==0 ) {
        b_opcode = 0b001100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "beq")==0 ) {
        b_opcode = 0b000100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);//PC <-- [PC] + 4 + ([I15]14 || [I15..0] || 02)
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "bgez")==0 ) {
        b_opcode = 0b000001;
        b_rt = 0b00001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "bgtz")==0 ) {
        b_opcode = 0b000111;
        b_rt = 0b00000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "blez")==0 ) {
        b_opcode = 0b000110;
        b_rt = 0b00000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "bltz")==0 ) {
        b_opcode = 0b000001;
        b_rt = 0b00000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "bne")==0 ) {
        b_opcode = 0b000101;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = (strToInt(s_reg)-4-current_pc)>>2&0b1111111111111111;
    }
    else if ( strcmp(operator, "lb")==0 ) {
        b_opcode = 0b100000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "lbu")==0 ) {
        b_opcode = 0b100100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "lh")==0 ) {
        b_opcode = 0b100001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "lhu")==0 ) {
        b_opcode = 0b100101;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "lui")==0 ) {
        b_opcode = 0b001111;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "lw")==0 ) {
        b_opcode = 0b100011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "lwc1")==0 ) {
        b_opcode = 0b110001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "ori")==0 ) {
        b_opcode = 0b001101;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "sb")==0 ) {
        b_opcode = 0b101000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "slti")==0 ) {
        b_opcode = 0b001010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "sltiu")==0 ) {
        b_opcode = 0b001011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else if ( strcmp(operator, "sh")==0 ) {
        b_opcode = 0b101001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "sw")==0 ) {
        b_opcode = 0b101011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "swc1")==0 ) {
        b_opcode = 0b111001;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", (");b_immediate = strToInt(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), "( )");b_rs = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "xori")==0 ) {
        b_opcode = 0b001110;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rt = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_rs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_immediate = strToInt(s_reg);
    }
    else
        b_opcode = 0xffffffff;
    if ( (b_rs|b_rt)==0xffffffff )
        return 0xffffffff;
    if(isDebug)  printf("I? %x %x %x %x\n", b_opcode, b_rs, b_rt, b_immediate);
    if ( b_opcode != 0xffffffff )
        return (b_opcode<<26) | (b_rs<<21) | (b_rt<<16) | (b_immediate&0x0000ffff);
    
    //J-Type Instructions
    b_target = 0;
    if ( strcmp(operator, "j")==0 ) {
        b_opcode = 0b000010;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_target = strToInt(s_reg)>>2&0b11111111111111111111111111;//<<2&0b0000111111111111111111111111100
    }
    else if ( strcmp(operator, "jal")==0 ) {
        b_opcode = 0b000011;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");
        b_target = strToInt(s_reg)>>2&0b11111111111111111111111111;
    }
    else
        b_opcode = 0xffffffff;
    if(isDebug)  printf("J? %x %x\n", b_opcode, b_target);
    if ( b_opcode != 0xffffffff )
        return (b_opcode<<26) | (b_target);
    
    //Coprocessor Instructions
    b_opcode = 0b010001;
    b_format = 0;
    b_ft = 0;
    b_fs = 0;
    b_fd = 0;
    b_func = 0xffffffff;
    if ( strcmp(operator, "add.s")==0 ) {
        b_func = 0b000000;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "cvt.s.w")==0 ) {
        b_func = 0b100000;
        b_format = 0b10100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "cvt.w.s")==0 ) {
        b_func = 0b100100;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    else if ( strcmp(operator, "div.s")==0 ) {
        b_func = 0b000011;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    if ( strcmp(operator, "mfc1")==0 ) {
        b_func = 0b000000;
        b_format = 0b00000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
    }
    if ( strcmp(operator, "mov.s")==0 ) {
        b_func = 0b000110;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
    }
    if ( strcmp(operator, "mtc1")==0 ) {
        b_func = 0b000000;
        b_format = 0b00100;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
    }
    if ( strcmp(operator, "mul.s")==0 ) {
        b_func = 0b000010;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    if ( strcmp(operator, "sub.s")==0 ) {
        b_func = 0b000001;
        b_format = 0b10000;
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fd = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_fs = registerToNumber(s_reg);
        i = get_string_assemble(&s_reg[0], command, i, sizeof(s_reg), ", ");b_ft = registerToNumber(s_reg);
    }
    else
        b_func = 0xffffffff;
    if(isDebug)  printf("C? %x %x %x %x %x %x\n", b_opcode, b_format, b_ft, b_fs, b_fd, b_func);
    if ( b_opcode != 0xffffffff )
        return (b_opcode<<26) | (b_format<<21) | (b_ft<<16) | (b_fs<<11) | (b_fd<<6) | b_func;
    
    return 0xffffffff;
}