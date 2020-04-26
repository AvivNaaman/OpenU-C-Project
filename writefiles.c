//
// Created by yotam on 26/04/2020.
//

#include <stdio.h>
#include <string.h>
#include "code.h"
#include "table.h"
#include "writefiles.h"
int write_ob(machine_word **code_img, int icf, int dcf, char *filename);

int write_output_files(machine_word **code_img, int icf, int dcf, char *filename, table ent_table, table ext_table) {
	return write_ob(code_img, icf, dcf, filename); /* TODO: Add externals/entries file writing */
}
int write_ob(machine_word **code_img, int icf, int dcf, char *filename){
    int val,i,j;
    FILE *file_desc;
    char *output_filename = malloc_with_check(strlen(filename) + 4);
    strcpy(output_filename,filename);
    strcat(output_filename,".ob");
    /* Try to open the file for writing */
    file_desc = fopen(output_filename, "w");
    if (file_desc == NULL ){
        printf("Can't create or rewrite to file %s.", filename);
        return TRUE;
    }
    fprintf(file_desc, "%d %d\n", icf - 100, dcf);
    for(i=0;code_img[i] != NULL;i++) {
        /*for(j=0;i<icf,i<11;i++,j++){*/
        	/* TODO: Check if it's possible to do it using pointer cast or something - it'll be much better! */
        	/* if it's a code word, build the binary data word: */
            if(code_img[i]->length != 0) {
                val = (code_img[i]->word.code->opcode << 18) | (code_img[i]->word.code->src_addressing << 16) |
                      (code_img[i]->word.code->src_register << 13) | (code_img[i]->word.code->dest_addressing << 11) |
                      (code_img[i]->word.code->dest_register << 8) | (code_img[i]->word.code->funct) << 3 |
                      (code_img[i]->word.code->ARE);
            }
            /* if it's a data word, build the binary data:  */
            else{
                val = (code_img[i]->word.data->data<<3)|(code_img[i]->word.data->ARE);
            }
            /* Write the value to the file - first */
            fprintf(file_desc, "%.7d %.6x\n", i + 100, val);
        /*}*/
    }
    fclose(file_desc);
}