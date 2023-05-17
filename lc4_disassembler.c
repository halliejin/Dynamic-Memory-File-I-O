/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"

int reverse_assemble (row_of_memory* memory) {
    row_of_memory* current_node;
    short unsigned int opcode = 0b0001;
    char assembly_str[20];

    current_node = search_opcode(memory, opcode);

    while (current_node != NULL) {
        // Extract bits from the contents field
        short unsigned int dr = (current_node->contents >> 9) & 0b111;
        short unsigned int sr1 = (current_node->contents >> 6) & 0b111;
        short unsigned int imm_flag = (current_node->contents >> 5) & 0b1;
        short unsigned int sub_opcode = (current_node->contents >> 3) & 0b111;
        short unsigned int sr2 = current_node->contents & 0b111;
        short int imm5 = current_node->contents & 0b11111;

        // Sign extend IMM5
        if (imm5 & (1 << 4)) {
            imm5 |= 0xFFF0;
        }

        if (imm_flag) {
            sprintf(assembly_str, "ADD R%d, R%d, #%d", dr, sr1, imm5);
        } else {
            // Decode the sub-opcode and generate the assembly string
            switch (sub_opcode) {
                case 0b000:
                    sprintf(assembly_str, "ADD R%d, R%d, R%d", dr, sr1, sr2);
                    break;
                case 0b001:
                    sprintf(assembly_str, "MUL R%d, R%d, R%d", dr, sr1, sr2);
                    break;
                case 0b010:
                    sprintf(assembly_str, "SUB R%d, R%d, R%d", dr, sr1, sr2);
                    break;
                case 0b011:
                    sprintf(assembly_str, "DIV R%d, R%d, R%d", dr, sr1, sr2);
                    break;
                default:
                    break;
            }
        }

        // Allocate memory for the assembly string and store it in the node's assembly field
        current_node->assembly = (char *) malloc(strlen(assembly_str) + 1);
        strcpy(current_node->assembly, assembly_str);

        // Search for the next node with the matching opcode
        current_node = search_opcode(current_node->next, opcode);
    }

    return 0;
}

