/*! \file
 *
 * This file contains definitions for an Arithmetic/Logic Unit of an
 * emulated processor.
 */


#include <stdio.h>
#include <stdlib.h>   /* malloc(), free() */
#include <string.h>   /* memset() */

#include "alu.h"
#include "instruction.h"


/*
 * This function dynamically allocates and initializes the state for a new ALU
 * instance.  If allocation fails, the program is terminated.
 */
ALU * build_alu() {
    /* Try to allocate the ALU struct.  If this fails, report error then exit. */
    ALU *alu = malloc(sizeof(ALU));
    if (!alu) {
        fprintf(stderr, "Out of memory building an ALU!\n");
        exit(11);
    }

    /* Initialize all values in the ALU struct to 0. */
    memset(alu, 0, sizeof(ALU));
    return alu;
}


/* This function frees the dynamically allocated ALU instance. */
void free_alu(ALU *alu) {
    free(alu);
}


/*
 * This function implements the logic of the ALU.  It reads the inputs and
 * opcode, then sets the output accordingly.  Note that if the ALU does not
 * recognize the opcode, it should simply produce a zero result.
 */
void alu_eval(ALU *alu) {
    uint32_t A, B, aluop;
    uint32_t result;

    A = pin_read(alu->in1);
    B = pin_read(alu->in2);
    aluop = pin_read(alu->op);

    result = 0;

    switch (aluop) {

        /* simple addition */
        case ALUOP_ADD:
            result = A + B;
            break;
        /* bitwise inversion */
        case ALUOP_INV:
            result = ~A;
            break;
        /* simple subtraction */
        case ALUOP_SUB:
            result = A - B;
            break;
        /* binary XOR of A and B */
        case ALUOP_XOR:
            result = A ^ B;
            break;
        /* binary OR of A and B */
        case ALUOP_OR:
            result = A | B;
            break;
        /* Incrementing a register */
        case ALUOP_INCR:
            result = A + 1;
            break;
        /* Bitwise and on A and B */
        case ALUOP_AND:
            result = A & B;
            break;
        /* Arithmetic shift right A */
        case ALUOP_SRA:
            result = (signed int)A >> 1;
            break;
        /* Logical shift right on A */
        case ALUOP_SRL:
            result = ((unsigned int)A) >> 1;
            break;
        /* Arithmetic shift left A by 1 */
        case ALUOP_SLA:
            result = A << 1;
            break;
        /* Logical shift left on A */
        case ALUOP_SLL:
            result = A << 1;
            break;
        default:
            result = 0;

    }

    pin_set(alu->out, result);
}
