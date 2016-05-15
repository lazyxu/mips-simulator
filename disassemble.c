//
//  disassemble.c
//  test
//
//  Created by Meteor on 2016/5/11.
//  Copyright © 2016年 Meteor. All rights reserved.
//

#include "disassemble.h"

 char reg_internal[32][5]={"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6","t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "k0", "k1", "gp", "sp", "fp", "ra" };
extern int isDebug;
int RDisassemble(int instruction)
{
    unsigned int rs,rt,rd,sa;
    rs = (instruction>>21)&0b11111;
    rt = (instruction>>16)&0b11111;
    rd = (instruction>>11)&0b11111;
    sa = (instruction>>6)&0b11111;
    switch ( instruction&0b111111 ) {
        case 0b100000:
            if ( sa==0 && rd!=0 ) printf("add    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b100001:
            if ( sa==0 && rd!=0 ) printf("addu   $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b100100:
            if ( sa==0 && rd!=0 ) printf("and    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b001101:
            if ( sa==0 && rd==0 && rs==0 && rt==0 ) printf("break     ");
            else printf("undefined  ");
            return 1;
        case 0b011010:
            if ( sa==0 && rd==0 ) printf("div    $%s,$%s  ", reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b011011:
            if ( sa==0 && rd==0 ) printf("divu   $%s,$%s  ", reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b001001:
            if ( sa==0 && rt==0 ) printf("jalr   $%s,$%s  ", reg_internal[rd], reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b001000:
            if ( sa==0 && rt==0 && rd==0 ) printf("jr   $%s  ", reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b010000:
            if ( sa==0 && rt==0 && rs==0 ) printf("mfhi   $%s  ", reg_internal[rd]);
            else printf("undefined  ");
            return 1;
        case 0b010010:
            if ( sa==0 && rt==0 && rs==0 ) printf("mflo   $%s  ", reg_internal[rd]);
            else printf("undefined  ");
            return 1;
        case 0b010001:
            if ( sa==0 && rt==0 && rd==0 ) printf("mthi   $%s  ", reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b010011:
            if ( sa==0 && rt==0 && rd==0 ) printf("mtlo   $%s  ", reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b011000:
            if ( sa==0 && rd==0 ) printf("mult   $%s,$%s  ", reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b011001:
            if ( sa==0 && rd==0 ) printf("multu  $%s,$%s  ", reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b100111:
            if ( sa==0 && rd!=0 ) printf("nor    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b100101:
            if ( sa==0 && rd!=0 ) printf("or     $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b000000:
            if ( rs==0 && rd!=0 ) printf("sll    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[sa]);
            else if ( instruction==0 ) printf("nop  ");
            else printf("undefined  ");
            return 1;
        case 0b000100:
            if ( sa==0 && rd!=0 ) printf("sllv   $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b101010:
            if ( sa==0 && rd!=0 ) printf("slt    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b101011:
            if ( sa==0 && rd!=0 ) printf("sltu   $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b000011:
            if ( rs==0 && rd!=0 ) printf("sra    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[sa]);
            else printf("undefined  ");
            return 1;
        case 0b000111:
            if ( sa==0 && rd!=0 ) printf("srav   $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b000010:
            if ( rs==0 && rd!=0 ) printf("srl    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[sa]);
            else printf("undefined  ");
            return 1;
        case 0b000110:
            if ( sa==0 && rd!=0 ) printf("srlv   $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rt], reg_internal[rs]);
            else printf("undefined  ");
            return 1;
        case 0b100010:
            if ( sa==0 && rd!=0 ) printf("sub    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b100011:
            if ( sa==0 && rd!=0 ) printf("subu    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
        case 0b001100:
            printf("syscall");
            return 1;
        case 0b100110:
            if ( sa==0 && rd!=0 ) printf("xor    $%s,$%s,$%s  ", reg_internal[rd], reg_internal[rs], reg_internal[rt]);
            else printf("undefined  ");
            return 1;
    }
    return 0;
}

int IDisassemble(int instruction, unsigned int current_pc)
{
    unsigned int rs,rt,immediate;
    rs = (instruction>>21)&0b11111;
    rt = (instruction>>16)&0b11111;
    immediate = instruction&0b1111111111111111;
    switch ( (instruction>>26)&0b111111 ) {
        case 0b001000:
            printf("addi   $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
        case 0b001001:
            printf("addiu  $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], (short int)immediate);
            return 1;
        case 0b001100:
            printf("andi   $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
        case 0b000100:
            printf("beq    $%s,$%s,0x%08x  ", reg_internal[rt], reg_internal[rs], current_pc+4+(int)(immediate<<2));
            return 1;
        case 0b000001:
            if ( (rt&0b111111)==1 )
                printf("bgqz   $%s,0x%08x  ", reg_internal[rs], current_pc+4+(int)(immediate<<2));
            else if ( (rt&0b111111)==0 )
                printf("bltz   $%s,0x%08x  ", reg_internal[rs], current_pc+4+(int)(immediate<<2));
            else
                printf("undenfined  ");
            return 1;
        case 0b000111:
            if ( (rt&0b111111)==0 )
                printf("bgtz   $%s,0x%08x  ", reg_internal[rs], current_pc+4+(int)(immediate<<2));
            else
                printf("undenfied  ");
            return 1;
        case 0b000110:
            if ( (rt&0b111111)==0 )
                printf("blez   $%s,0x%08x  ", reg_internal[rs], current_pc+4+(int)(immediate<<2));
            else
                printf("undenfied  ");
            return 1;
        case 0b000101:
            printf("bne    $%s,$%s,0x%08x  ", reg_internal[rt], reg_internal[rs], current_pc+4+(int)(immediate<<2));
            return 1;
        case 0b100000:
            printf("lb     $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b100100:
            printf("lbu    $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b100001:
            printf("lh     $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b100101:
            printf("lhu    $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b001111:
            printf("lui    $%s,%d  ", reg_internal[rt], immediate);
            return 1;
        case 0b100011:
            printf("lw     $%s,%d($%s)  ", reg_internal[rt], (short int)immediate, reg_internal[rs]);
            return 1;
        case 0b110001:
            printf("lwc1   $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b001101:
            printf("ori   $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
        case 0b101000:
            printf("sb     $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b001010:
            printf("slti   $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
        case 0b001011:
            printf("sltiu  $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
        case 0b101001:
            printf("sh     $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b101011:
            printf("sw     $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b111001:
            printf("swc1   $%s,%d($%s)  ", reg_internal[rt], immediate, reg_internal[rs]);
            return 1;
        case 0b001110:
            printf("xori   $%s,$%s,%d  ", reg_internal[rt], reg_internal[rs], immediate);
            return 1;
    }
    return 0;
}

int JDisassemble(int instruction, unsigned int current_pc)
{
    unsigned int target;
    target = instruction&0b11111111111111111111111111;
    switch ( instruction>>26 ) {
        case 0b000010:
            printf("j      0x%08x  ", (current_pc&0xf000000) | target<<2);//&0b0000111111111111111111111111100 PC <-- [PC31..28] || [I25..0] || 02
            return 1;
        case 0b000011:
            printf("jal    0x%08x  ", (current_pc&0xf000000) | target<<2);
            return 1;
    }
    return 0;
}

int CoprocessorDisassemble(int instruction)
{
    int format,ft,fs,fd,func;
    format = (instruction>>21)&0b11111;
    ft = (instruction>>16)&0b11111;
    fs = (instruction>>11)&0b11111;
    fd = (instruction>>6)&0b11111;
    func = instruction&0b111111;
    switch ( (func<<6 | format) & 0b11111111111 ) {
        case 0b00000010000:
            if ( fd!=0 ) printf("add.s   $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
        case 0b10000010100:
            if ( fd!=0 ) printf("cvt.s.w $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
        case 0b10010010000:
            if ( fd!=0 ) printf("cvt.w.s $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
        case 0b00001110000:
            if ( fd!=0 ) printf("div.s   $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
        case 0b00000000000:
            if ( fd==0 ) printf("mfcl   $%s,$%s  ", reg_internal[ft], reg_internal[fs]);
            else printf("undefined  ");
            return 1;
        case 0b00011010000:
            if ( ft==0 ) printf("mov.s   $%s,$%s  ", reg_internal[fd], reg_internal[fs]);
            else printf("undefined  ");
            return 1;
        case 0b00000000100:
            if ( fd==0 ) printf("mtcl   $%s,$%s  ", reg_internal[ft], reg_internal[fs]);
            else printf("undefined  ");
            return 1;
        case 0b00001010000:
            if ( fd!=0 ) printf("mul.s   $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
        case 0b00000110000:
            if ( fd!=0 ) printf("sub.s   $%s,$%s,$%s  ", reg_internal[fd], reg_internal[fs], reg_internal[ft]);
            else printf("undefined  ");
            return 1;
    }
    return 0;
}

int Disassemble(int instruction, unsigned int current_pc)
{
    int x = instruction>>26;
    if ( x==0 ) {
        return RDisassemble(instruction);
    }
    else if ( x>>1==1 ) {
        return JDisassemble(instruction, current_pc);
    }
    else  if ( x>>2==4 ) {
        return CoprocessorDisassemble(instruction);
    }
    else {
        return IDisassemble(instruction, current_pc);
    }
    return 0;
}