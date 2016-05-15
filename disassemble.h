//
//  disassemble.h
//  test
//
//  Created by Meteor on 2016/5/11.
//  Copyright © 2016年 徐亮. Meteor rights reserved.
//

#ifndef disassemble_h
#define disassemble_h

#include <stdio.h>

int RDisassemble(int instruction);
int IDisassemble(int instruction, unsigned int current_pc);
int JDisassemble(int instruction, unsigned int current_pc);
int CoprocessorDisassemble(int instruction);
int Disassemble(int instruction, unsigned int current_pc);

#endif /* disassemble_h */
