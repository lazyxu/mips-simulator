//
//  assemble.h
//  test
//
//  Created by Meteor on 2016/5/11.
//  Copyright © 2016年 Meteor. All rights reserved.
//

#ifndef assemble_h
#define assemble_h

#include <stdio.h>
//int RAssemble(int instruction);
//int IAssemble(int instruction);
//int JAssemble(int instruction);
//int CoprocessorAssemble(int instruction);
int get_string_assemble(char* getstr, char* string, int n, int max_len, char *skip_char);
int Assemble(char *instruction, unsigned int current_pc);

#endif /* assemble_h */
