/* Contains a method for processing a line in the first pass and another for processing a line in the second pass. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processline.h"

/*
 * processes a single instruction line in first pass. returns whether an error occurred.
 * line - the text line from .asm file.
 * datas, code, externals - symbol tables for .data, .code and .external instruction.
 * IC - a pointer to the instruction counter.
 * DC - a pointer to the data counter.
 * code_img - the code image array.
 * data_img - the data image array.
 */
int
process_line_fpass(char *line, table *datas, table *codes, table *externals, int *IC, int *DC, machine_word **code_img,
                   machine_data **data_img) {
    int i;
    char symbol[32];
    instruction_type instruction;
    /* TODO: implement */

    i = 0;

    MOVE_TO_NOT_WHITE(line, i) /* Move to next non-white char */
    if (line[i] == '\n' || line[i] == EOF || line[i] == ';')
        return FALSE; /* Empty/Comment line - no errors found (of course) */

    /* Check if symbol (*:), stages 1.3-1.5 */
    /* if tried to define label, but it's invalid, return that an error occurred. */
    if (parse_symbol(line, symbol)) {
        return TRUE;
    }

    if (symbol[0] != '\0') {
        for (; line[i] != ':'; i++); /* is symbol detected, start analyzing from it's deceleration end */
        i++;
    }

    MOVE_TO_NOT_WHITE(line, i) /* Move to next not-white char */

    /* Check if it's an instruction (starting with '.') */
    instruction = find_instruction_from_index(line, &i);

    MOVE_TO_NOT_WHITE(line, i);

    /* is it's an instruction */
    if (instruction != NONE_INST) {
        /* if .string or .data, and symbol defined, put it into the symbol table */
        if ((instruction == DATA || instruction == STRING) && symbol[0] != '\0')
            /* is data or string, add DC with the symbol to the table */
            add_table_item(datas, symbol, *DC);
        /* if string, encode into data image buffer and increase dc as needed. */
        if (instruction == STRING)
            (*DC) += process_string_instruction(line, i, data_img, DC);
            /* if .data, do same but parse numbers. */
        else if (instruction == DATA)
            (*DC) += process_data_instruction(line, i, data_img, DC);
            /* if .extern, add to externals symbol table */
        else if (instruction == EXTERN)
            add_table_item(externals, symbol, *DC);
        /* .entry is handled in second pass! */
    } /* end if (instruction != NONE) */
        /* not instruction=>it's a command! */
    else {
        /* if symbol defined, add it to the table */
        if (symbol[0] != '\0')
            add_table_item(codes, symbol, *IC);
        /* Analyze code */
        process_code(line, i, IC, code_img);
    }
    return FALSE;
}

/*
 * Processes a single instruction line in second pass.
 * returns whether error occurred.
 * line - the line from .as file
 * tables - all symbol tables that are needed
 * ic - pointer to IC counter
 */
int process_line_spass(char *line, table *ent_table, table *code_table, int *ic, table ext_table, table data_table,
                       machine_word **code_img) { /*TODO: DEBUG*/
    int i;
    char *indexOfColon;
    char *token;
    MOVE_TO_NOT_WHITE(line,i)
    if(line[i]==';') return FALSE;
    indexOfColon = strchr(line, ':');
    /*check for label */
    if (indexOfColon != NULL) {
        i = indexOfColon - line;
        i++;
    }
    /* if it's guide line*/
    if (line[i] == '.') {
        /*if it's entry we add it to the symbol table*/
        if (strncmp(".entry", line, 6) == 0) {
            i += 6;
            MOVE_TO_NOT_WHITE(line, i);
            token = strtok(line, " ");
            /*if label is already in table dont add it*/
            if (find_by_key(&ent_table, token) == NULL) {
                int val;
                token = strtok(NULL, "\n"); /*get name of label*/
                val = find_by_key(data_table, token)->value;
                add_table_item(ent_table, token, val);
            }
        }
        return FALSE;
    }
    if (add_symbols_to_code(line, ic, code_img, code_table, data_table,ext_table)) {
        return TRUE;
    }
    return FALSE;
}