#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "disassemble.h"
#include "assemble.h"
#define bits unsigned int //以32位为例
#define MEMORY_SIZE 1024
#define START_PC 0x00400000

bits instruction;
char reg[32][5]={"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
                "t0", "t1", "t2", "t3", "t4", "t5", "t6","t7",
                "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
                "s8", "s9", "k0", "k1", "gp", "sp", "fp", "ra" };
bits reg_value[32]={0};
char operator[10];
bits rs, rt, rd, sa, immediate, target, format, ft, fs, fd, func;
bits pc, head_pc;
bits hi, lo;//乘法除法模块
bits memory[MEMORY_SIZE]={0};
bits bp[100];
bits bp_index=0;
int show_screen=0;
int memory_index;
int isDebug=0;

extern int lable_address[30];
extern char lable[30][20];
extern int lable_index;
//0x80000000到0xFFFFFFFF的一段为Kernel Space，仅限于Kernel Mode访问

int input_2_string(char *command)
{
    int i;
    for (i=31; i>=0; i--) {
        if ( command[i] == '0' || command[i] =='1') {
            instruction = instruction | ( (command[i] - '0')&1 )<<(31-i);
        }
        else {
            printf("invaid instruction!\n");
            return 0;
        }
    }
    return 1;
}

int input_16_string(char *command)
{
    int i;
    for (i=7; i>=0; i--) {
        if ( command[i] >= '0' && command[i] <= '9' ) {
            instruction = instruction | ( (command[i] - '0')&0xf )<<((7-i)*4);
        }
        else if ( command[i] >= 'A' && command[i] <= 'F' ) {
            instruction = instruction | (( (command[i] - 'A')+10)&0xf )<<((7-i)*4);
        }
        else if ( command[i] >= 'a' && command[i] <= 'f' ) {
            instruction = instruction | (( (command[i] - 'a')+10)&0xf )<<((7-i)*4);
        }
        else {
            printf("invaid instruction!\n");
            break;
        }
    }
    return 1;
}

unsigned int bufferReadFile(char* filename, char** buffer) {
    unsigned int buff_size = MEMORY_SIZE;
    unsigned int len = 0;
    unsigned int numberOfRead = 0;
    char *content = (char*)malloc(buff_size);
    FILE* fp = fopen(filename, "rb");
    unsigned char* buff;
    if (fp == NULL) {
        printf("Open file failed.");
    }
    numberOfRead = fread(content, 1, buff_size, fp);
    len += numberOfRead;
    buff_size <<= 1;
    buff = (unsigned char*)malloc(buff_size);
    while (1) {
        numberOfRead = fread(buff, 1, buff_size, fp);
        len += numberOfRead;
        content = (char*)realloc(content, (buff_size << 1) - MEMORY_SIZE);
        memcpy(content + buff_size - MEMORY_SIZE, buff, buff_size);
        free(buff);
        if (numberOfRead != buff_size) break;
        buff_size <<= 1;
        buff = (unsigned char*)malloc(buff_size);
    }
    fclose(fp);
    *buffer = content;
    return len;
}

int read_file(char *filename)
{
    char *content;
    char padding[4]="\xcc\xcc\xcc\xcc";
    int len = bufferReadFile(filename, &content);
    printf("file length: %d bytes\n", len);
    if ( len > 1024 ) {
        printf("file too big");
        exit(1);
    }
    if ( len == 0 ) {
        printf("no data");
        exit(1);
    }
    memcpy(memory, content, len*sizeof(char));
    if ( len < 1024 )
        memcpy(memory+len/4+1, padding, 4*sizeof(char));
    free(content);
    return len;
}

int R_execute()
{
    rs = (instruction>>21)&0b11111;
    rt = (instruction>>16)&0b11111;
    rd = (instruction>>11)&0b11111;
    sa = (instruction>>6)&0b11111;
    switch ( instruction&0b111111 ) {
        case 0b100000://add rd,rs,rt
            reg_value[rd] = reg_value[rs] + reg_value[rt];
            pc += 4;
            return 1;
        case 0b100001://addu rd,rs,rt
            reg_value[rd] = (int)reg_value[rs] + (int)reg_value[rt];
            pc += 4;
            return 1;
        case 0b100100://and rd,rs,rt
            reg_value[rd] = reg_value[rs] & reg_value[rt];
            pc += 4;
            return 1;
        case 0b001101://break
            bp[bp_index]=pc;
            bp_index++;
            pc += 4;
            return 1;
        case 0b011010://div rs,rt
            hi = (int)reg_value[rs] / (int)reg_value[rt];
            lo = (int)reg_value[rs] % (int)reg_value[rt];
            pc += 4;
            return 1;
        case 0b011011://divu rs,rt
            hi = reg_value[rs] / reg_value[rt];
            lo = reg_value[rs] % reg_value[rt];
            pc += 4;
            return 1;
        case 0b001001://jalr rd,rs
            reg_value[rd] = pc + 4;
            pc = reg_value[rs] << 4;
            return 1;
        case 0b001000://jr rs
            pc = reg_value[rs] << 4;
            return 1;
        case 0b010000://mfhi rd
            reg_value[rd] = hi;
            pc += 4;
            return 1;
        case 0b010010://mflo rd
            reg_value[rd] = lo;
            pc += 4;
            return 1;
        case 0b010001://mthi rs
            hi = reg_value[rd];
            pc += 4;
            return 1;
        case 0b010011://mtlo rs
            lo = reg_value[rd];
            pc += 4;
            return 1;
        case 0b011000://mult rs,rt
            hi = (((long)reg_value[rs] * (long)reg_value[rt])>>32)&0xffff;
            lo = ((long)reg_value[rs] * (long)reg_value[rt])&0xffff;
            pc += 4;
            return 1;
        case 0b011001://multu rs,rt
            hi = (((unsigned long)reg_value[rs] * (unsigned long)reg_value[rt])>>32)&0xffff;
            lo = ((unsigned long)reg_value[rs] * (unsigned long)reg_value[rt])&0xffff;
            pc += 4;
            return 1;
        case 0b100111://nor rd,rs,rt
            reg_value[rd] = !(reg_value[rs] | reg_value[rt]);
            pc += 4;
            return 1;
        case 0b100101://or rd,rs,rt
            reg_value[rd] = reg_value[rs] | reg_value[rt];
            pc += 4;
            return 1;
        case 0b000000://sll rd,rt,sa
            if ( rs==0 && rd!=0 )
                reg_value[rd] = reg_value[rt] << sa;
            pc += 4;
            return 1;
        case 0b000100://sllv rd,rt,rs
            reg_value[rd] = reg_value[rt] << reg_value[rs];
            pc += 4;
            return 1;
        case 0b101010://slt rd,rs,rt
            reg_value[rd] = ((int)reg_value[rs]<(int)reg_value[rt])?1:0;
            pc += 4;
            return 1;
        case 0b101011://sltu rd,rs,rt
            reg_value[rd] = (reg_value[rs]<reg_value[rt])?1:0;
            pc += 4;
            return 1;
        case 0b000011://sra rd,rt,sa
            reg_value[rd] = (int)reg_value[rt] >> sa;//算术右移
            pc += 4;
            return 1;
        case 0b000111://srav rd,rt,rs
            reg_value[rd] = (int)reg_value[rt] >> reg_value[rs];
            pc += 4;
            return 1;
        case 0b000010://srl rd,rt,sa
            reg_value[rd] = reg_value[rt] >> sa;//逻辑右移
            pc += 4;
            return 1;
        case 0b000110://srlv rd,rt,rs
            reg_value[rd] = reg_value[rt] >> reg_value[rs];
            pc += 4;
            return 1;
        case 0b100010://sub rd,rs,rt
            reg_value[rd] = (int)reg_value[rs] - (int)reg_value[rt];
            pc += 4;
            return 1;
        case 0b100011://subu rd,rs,rt
            reg_value[rd] = reg_value[rs] - reg_value[rt];
            pc += 4;
            return 1;
        case 0b001100://syscall  ????????????????????????????????????????????
            
            pc += 4;
            return 1;
        case 0b100110://xor rd,rs,rt
            reg_value[rd] = reg_value[rs] ^ reg_value[rt];
            pc += 4;
            return 1;
    }
    pc += 4;
    return 0;
}

int I_execute()
{
    char *pByte = &memory[0];
    rs = (instruction>>21)&0b11111;
    rt = (instruction>>16)&0b11111;
    immediate = instruction&0b1111111111111111;
    switch ( (instruction>>26)&0b111111 ) {
        case 0b001000://addi rt,rs,immediate
            reg_value[rt] = (int)reg_value[rs] + (int)immediate;
            pc += 4;
            return 1;
        case 0b001001://addiu rt,rs,immediate
            reg_value[rt] = reg_value[rs] + immediate;
            pc += 4;
            return 1;
        case 0b001100://andi rs,rt,immediate
            reg_value[rt] = reg_value[rs] & immediate;
            pc += 4;
            return 1;
        case 0b000100://beq rs,rt,immediate
            if( reg_value[rs]==reg_value[rt] )
                pc = pc + 4 + (short int)immediate;
            else
                pc = pc + 4;
            return 1;
        case 0b000001:
            if ( rt&0b111111 ) {//bgeq rs,immediate
                if( reg_value[rs]>=reg_value[rt] )
                    pc = pc + 4 + (short int)immediate;
                else
                    pc = pc + 4;
            }
            else if ( (rt&0b111111)==0 ) {//bltz rs,immediate
                if( (int)reg_value[rs]<0 )
                    pc = pc + 4 + (short int)immediate;
                else
                    pc = pc + 4;
            }
            return 1;
        case 0b000111:
            if ( (rt&0b111111)==0 ) {//bgtz rs,immediate
                if( (int)reg_value[rs]>0 )
                    pc = pc + 4 + (short int)immediate;
                else
                    pc = pc + 4;
            }
            return 1;
        case 0b000110:
            if ( (rt&0b111111)==0 ) {//blez rs,immediate
                if( (int)reg_value[rs]<=0 )
                    pc = pc + 4 + (short int)immediate;
                else
                    pc = pc + 4;
            }
            return 1;
        case 0b000101://bne rs,rt,immediate
            if( reg_value[rs]!=reg_value[rt] )
                pc = pc + 4 + (short int)immediate;
            else
                pc = pc + 4;
            return 1;
        case 0b100000://lb rt,immediate(rs)
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)) & 0xff;
            //rt = (int)(*(pByte + (reg_value[rs] - START_PC + (short int)immediate)));
            pc += 4;
            return 1;
        case 0b100100://lbu rt,immediate(rs)   ???
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)) & 0xff;
            pc += 4;
            return 1;
        case 0b100001://lh rt,immediate(rs)
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) & 0xff;
            pc += 4;
            return 1;
        case 0b100101://lhu rt,immediate(rs)
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) & 0xff;
            pc += 4;
            return 1;
        case 0b001111://lui rt,immediate  load constant halfword c into upper halfword of register r
            reg_value[rt] = immediate << 16;
            pc += 4;
            return 1;
        case 0b100011://lw rt,immediate(rs)  Rt <-- M{[Rs] + [I15]16 || [I15..0]} 暂时不考虑特殊情况
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) & 0xff;
            reg_value[rt] = (reg_value[rt]<<8) | (*(pByte+(reg_value[rs] + (int)immediate - START_PC)) & 0xff);
            pc += 4;
            return 1;
        case 0b110001://lwcl rt,immediate(rs) load word into co-processor 和上一句有啥区别？
            reg_value[rt] = *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) & 0xff;
            reg_value[rt] = (reg_value[rt]<<8) | (*(pByte+(reg_value[rs] + (int)immediate - START_PC)) & 0xff);
            pc += 4;
            return 1;
        case 0b001101://ori rt,rs,immediate
            reg_value[rt] = reg_value[rs] | immediate;
            pc += 4;
            return 1;
        case 0b101000://sb rt,immediate(rs)
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)) = reg_value[rt] & 0xff;
            pc += 4;
            return 1;
        case 0b001010://slti rt,rs,immediate
            reg_value[rt] = ((short int)reg_value[rs]<(short int)immediate)?1:0;
            pc += 4;
            return 1;
        case 0b001011://sltiu rt,rs,immediate
            reg_value[rt] = (reg_value[rs]<immediate)?1:0;
            pc += 4;
            return 1;
        case 0b101001://sh rt,immediate(rs)
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) = reg_value[rt] & 0xff;
            pc += 4;
            return 1;
        case 0b101011://sw rt,immediate(rs)
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)) = reg_value[rt] & 0xff;
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) = (reg_value[rt]>>8) & 0xff;
            pc += 4;
            return 1;
        case 0b111001://swcl rt,immediate(rs)
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)) = reg_value[rt] & 0xff;
            *(pByte+(reg_value[rs] + (int)immediate - START_PC)+1) = (reg_value[rt]>>8) & 0xff;
            pc += 4;
            return 1;
        case 0b001110://xori rt,rs,immediate
            reg_value[rt] = reg_value[rs] ^ immediate;
            pc += 4;
            return 1;
    }
    pc += 4;
    return 0;
}

int J_execute()
{
    target = instruction&0b11111111111111111111111111;
    switch ( instruction>>26 ) {
        case 0b000010://j target<<4  pc <- (pc+4)[31,28] || target || ‘00’
            pc = ((pc+4)&0xf0000000) | (target<<2);
            return 1;
        case 0b000011://jal target<<4
            pc = ((pc+4)&0xf0000000) | (target<<2);
            reg_value[31] = pc + 8;
            return 1;
    }
    pc += 4;
    return 0;
}

int Coprocessor_execute()
{
    format = (instruction>>21)&0b11111;
    ft = (instruction>>16)&0b11111;
    fs = (instruction>>11)&0b11111;
    fd = (instruction>>6)&0b11111;
    func = instruction&0b111111;
    switch ( (func<<6 | format) & 0b11111111111 ) {
        case 0b00000010000://add.s fd.fs.ft
            reg_value[fd] = (float)reg_value[fs] + (float)reg_value[ft];
            pc += 4;
            return 1;
        case 0b10000010100://cvt.s.w fd.fs.ft
            
            pc += 4;
            return 1;
        case 0b10010010000://cvt.w.s fd.fs.ft
            
            pc += 4;
            return 1;
        case 0b00001110000://div.s fd.fs.ft
            reg_value[fd] = (float)reg_value[fs] / (float)reg_value[ft];
            pc += 4;
            return 1;
        case 0b00000000000://mfcl ft,fs
            
            pc += 4;
            return 1;
        case 0b00011010000://mov.s fd,fs
            reg_value[fd] = (float)reg_value[fs];
            pc += 4;
            return 1;
        case 0b00000000100://mtcl ft,fs
            
            pc += 4;
            return 1;
        case 0b00001010000://mul.s fd.fs.ft
            reg_value[fd] = (float)reg_value[fs] * (float)reg_value[ft];
            pc += 4;
            return 1;
        case 0b00000110000://sub.s fd.fs.ft
            reg_value[fd] = (float)reg_value[fs] - (float)reg_value[ft];
            pc += 4;
            return 1;
    }
    pc += 4;
    return 0;
}

int execute()
{
    int x = instruction>>26;
    if ( x==0 ) {
        return R_execute();
    }
    else if ( x>>1==1 ) {
        return J_execute();
    }
    else  if ( x>>2==4 ) {
        return Coprocessor_execute();
    }
    else {
        return I_execute();
    }
    return 0;
}

//mips是big-endian的mips架构，0x1234abcd在内存中的存放顺序是0x00000000-0x12,0x00000001-0x34,0x00000002-0xab,0x00000003-0xcd
void print_screen()
{
    int i, j;
    char *pChar;
    int address=START_PC;
    int temp_memory_index = memory_index;
    printf("$pc:0x%08x   ", pc);
    printf("$hi:0x%08x   ", hi);
    printf("$lo:0x%08x   \n", lo);
    for (i=0; i<32; i++) {
        printf("$%s:0x%08x   ", reg[(bits)i], reg_value[i]);
        if( (i+1)%4==0 )
            printf("\n");
    }
    pChar = &memory[temp_memory_index];
    for (i=0; i<4; i++) {
        printf("0x%08x   ", address);
        for (j=0; j<0x10; j++) {
            printf("%02x ", *(pChar+j)&0xff);
        }
        printf("  ");
        for (j=0; j<0x10; j++) {
            if ( (*(pChar+j)&0xff) >= 0x20 && (*(pChar+j)&0xff) <= 0x80 )
                printf("%c", *(pChar+j)&0xff);
            else
                printf(" ");
        }
        pChar += 0x10;
        address += 0x10;
        printf("\n");
    }

    head_pc = (pc-5*4<START_PC)?START_PC:(pc-5*4);
    address = head_pc;
    temp_memory_index = (head_pc-START_PC)/4;
    for (i=0; i<11; i++) {
        if ( address != pc )
            printf("  0x%08x  ", address);
        else
            printf("->0x%08x  ", address);
        if ( temp_memory_index >= MEMORY_SIZE || temp_memory_index < 0) {
            printf("invaild_address\n");
            exit(0);
        }
        printf("%08x  ", memory[temp_memory_index]);
        instruction = memory[temp_memory_index];
        if( !Disassemble(instruction, address) )
            printf("undefined");
        printf("\n");
        temp_memory_index += 1;
        address += 4;
    }
}

char *sub_string(char* string, int start, int end)
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

char *get_string(char* string, int n, int *x, int max_len, char *stop_char)
{
    char *getstr;
    getstr = malloc(sizeof(char)*max_len);
    int i, j, index=0;
    for (i=0; i<max_len; i++) {
        j = 0;
        *x = n+index;
        while ( stop_char[j] != '\0') {
            if( string[n+index] == stop_char[j] ) {
                getstr[index] = '\0';
                return getstr;
            }
            j++;
        }
        if( string[n+index] == '\0' ) {
            getstr[index] = '\0';
            return getstr;
        }
        getstr[index] = string[n+index];
        index++;
    }
    return getstr;
}

int check_reg(char *str)
{
    int i;
    for (i=0; i<32; i++) {
        if ( strcmp(str, reg[i])==0 )
            return i;
    }
    return -1;
}

int str2int(char *str, bits* value)
{
    int i;
    bits n=0;
    for (i=0; str[i]!='\0'; i++) {
        n = n * 16;
        if ( str[i]>='0' && str[i]<='9' )
            n += str[i] - '0';
        else if ( str[i]>='a' && str[i]<='f' )
            n += str[i] - 'a' + 10;
        else if ( str[i]>='A' && str[i]<='F' )
            n += str[i] - 'A' + 10;
        else {
            printf("invaild hex");
            return 0;
        }
    }
    *value = n;
    return 1;
}

int get_command()
{
    char ch;
    int i;
    int set_reg;
    char command[50]={0};
    bits set_reg_value;
    show_screen = 0;
    fgets(command, sizeof(command), stdin);// 当用方向键移动 delete修改后失效? gets(command);不安全
    command[strlen(command)-1]='\0';
    while ( !show_screen ) {
        show_screen = 1;
        if ( strcmp(command, "q")==0 )
            return 0;
        if ( strcmp(command, "run")==0 ) { //run
            show_screen = 0;
            if ( (pc-START_PC)/4>=MEMORY_SIZE ){
                print_screen();
                printf("undefined commond\n");
                exit(0);
            }
            instruction = memory[(pc-START_PC)/4];
            if ( !execute() ) {
                print_screen();
                printf("undefined commond\n");
                exit(0);
            }
            for (i=0; i<=bp_index; i++) {
                if ( pc == bp[bp_index] ) {
                    show_screen = 1;
                    break;
                }
            }
        }
        else {
            if ( strcmp(command, "next")==0 ) { //next
                instruction = memory[(pc-START_PC)/4];
                if ( !execute() ) {
                    print_screen();
                    printf("undefined commond\n");
                    continue;
                }
                break;
            }
            if ( strcmp(sub_string(command, 0, 3), "set ")==0 ) { //set $zero=134
                i = 4;
                while ( (ch = command[i])!='$') {
                    i++;
                }
                if ( (set_reg=check_reg(get_string(command, i+1, &i, 10, "="))) == -1)
                    printf("error command\n");
                else {
                    if ( str2int(get_string(command, i+1, &i, 10, "\r\n\0"), &set_reg_value) != 0 ) {
                        reg_value[set_reg] = set_reg_value;
                        break;
                    }
                }
            }
            fgets(command, sizeof(command), stdin);// 当用方向键移动 delete修改后失效? gets(command);不安全
            command[strlen(command)-1]='\0';
            show_screen = 0;
        }
    }
    return 1;
}

int main(void)
{
    printf("%lu位MIPS模拟器\n", sizeof(reg_value[0])*8);
    char path[50];
    //read_file("/Users/meteor/desktop/计算机原理/mips/test");
    FILE *des;
    char deschar[4];
    pc = START_PC;
    int choice;
    int i,len;
    bits address;
    head_pc = pc;
    reg_value[29]=START_PC+0x100000;
    memory_index=0;
    char command[100];
    while(1) {
        printf("请选择  \n1.单条汇编代码转二进制   2.单条二进制代码转汇编   \n3.汇编文件转二进制   4.二进制文件转汇编   5.调试二进制\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("单条汇编代码转二进制...\n");
                printf("输入q退出\n");
                getchar();
                while (1) {
                    fgets(command, sizeof(command), stdin);// 当用方向键移动 delete修改后失效? gets(command);不安全
                    command[strlen(command)-1]='\0';
                    if ( command[0]=='\0' ) {
                        continue;
                    }
                    instruction = Assemble(command, START_PC);
                    if ( instruction==0xffffffff)
                        printf("invaild command\n");
                    else if (instruction==0xffffff00)
                        break;
                    else
                        printf("%08x\n", instruction);
                }
                break;
            case 2:
                printf("单条二进制代码转汇编...\n");
                printf("请选择   \n1.二进制（不需要加0b）   \n2.十六进制（不需要加0x）\n");
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        printf("输入q退出\n");
                        while(1) {
                            scanf("%s", command);
                            command[32]='\0';
                            instruction = 0;
                            if ( strcmp(command, "q")==0 )
                                break;
                            if ( !input_2_string(command) )
                                continue;
                            if( !Disassemble(instruction, START_PC) )
                                printf("undefined\n");
                        }
                        break;
                    case 2:
                        while(1) {
                            scanf("%p", &instruction);
                            if( !Disassemble(instruction, START_PC) )
                                printf("undefined\n");
                        }
                        break;
                }
                break;
            case 3:
                printf("汇编文件转二进制...\n");
                printf("输入q退出\n");
                
                //char filename[100];
                //scanf("%s", filename);
                char *filename="/Users/meteor/Downloads/test.asm";
                
                FILE* fp = fopen(filename, "rb");
                if (fp == NULL) {
                    printf("Open file failed.");
                    break;
                }
                address = START_PC;
                while ((fgets (command, sizeof(command), fp)) != NULL)
                {
                    if (command[strlen(command)-1]=='\n')
                        command[strlen(command)-1] = '\0';
                    if ( command[strlen(command)-1]==':' ) {
                        command[strlen(command)-1] = '\0';
                        strcpy(lable[lable_index], command);
                        lable_address[lable_index] = address;
                        
                        lable_index++;
                    }
                    else
                        address += 4;
                }
                fclose(fp);
                fp = fopen(filename, "rb");
                des=fopen("/Users/meteor/Downloads/test.bin", "wb");
                address = START_PC;
                while ((fgets (command, sizeof(command), fp)) != NULL)
                {
                    if (command[strlen(command)-1]=='\n')
                        command[strlen(command)-1]='\0';
                    instruction = Assemble(command, address);
                    if ( instruction==0xfffffff0)
                        continue;
                    else if (instruction==0xffffff00) //  q
                        break;
                    if ( address != pc )
                        printf("0x%08x  ", address);
                    else
                        printf("0x%08x  ", address);
                    printf("%08x  ", instruction);
                    fwrite(&instruction,sizeof(char),4,des);
                    if( !Disassemble(instruction, address) )
                        printf("undefined  ");
                    printf("   <<   ");
                    fputs (command, stdout);
                    printf("\n");
                    address += 4;
                }
                fclose(des);
                fclose(fp);
                break;
            case 4:
                printf("二进制文件转汇编，请输入文件路径...\n");
                //scanf("%s", path);
                //len = read_file(path);//   /Users/meteor/Downloads/test
                len = read_file("/Users/meteor/Downloads/test");
                address = START_PC;
                memory_index = 0;
                for (i=0; i<len/4; i++) {
                    if ( address != pc )
                        printf("0x%08x  ", address);
                    else
                        printf("0x%08x  ", address);
                    printf("%08x  ", memory[memory_index]);
                    instruction = memory[memory_index];
                    if( !Disassemble(instruction, address) )
                        printf("undefined");
                    printf("\n");
                    memory_index += 1;
                    address += 4;
                }
                break;
            case 5:
                printf("调试二进制，请输入文件路径...\n");
                //scanf("%s", path);
                //read_file(path);//   /Users/meteor/Downloads/des
                read_file("/Users/meteor/Downloads/test.bin");
                printf("输入q退出\n");
                while(1) {
                    print_screen();
                    if ( !get_command() )
                        break;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}