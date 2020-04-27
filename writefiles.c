//
// Created by yotam on 26/04/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code.h"
#include "table.h"
#include "writefiles.h"

int write_ob(machine_word **code_img, machine_data **data_img, int icf, int dcf, char *filename);
int write_table_to_file(table tab, char *filename, char *file_extension);

int write_output_files(machine_word **code_img,  machine_data **data_img, int icf, int dcf, char *filename, table ent_table, table ext_table) {
	return write_ob(code_img, data_img, icf, dcf, filename) ||
	write_table_to_file(ext_table, filename, "ext") ||
	write_table_to_file(ent_table, filename, "ent");
}

int write_ob(machine_word **code_img, machine_data **data_img, int icf, int dcf, char *filename){
    int val,i,j;
    FILE *file_desc;
    char *output_filename = malloc_with_check(strlen(filename) + 4);
    strcpy(output_filename,filename);
    strcat(output_filename,".ob");
    /* Try to open the file for writing */
    file_desc = fopen(output_filename, "w");
	free(output_filename);
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
                val = 0;
                val = (code_img[i]->word.code->opcode << 18) | (code_img[i]->word.code->src_addressing << 16) |
                      (code_img[i]->word.code->src_register << 13) | (code_img[i]->word.code->dest_addressing << 11) |
                      (code_img[i]->word.code->dest_register << 8) | (code_img[i]->word.code->funct) << 3 |
                      (code_img[i]->word.code->ARE);
            }
            /* if it's a data word, build the binary data:  */
            else{
                val =0;
                val = (code_img[i]->word.data->data<<3)|(code_img[i]->word.data->ARE);
            }
            /* Write the value to the file - first */
            fprintf(file_desc, "%.7d %.6x\n", i + 100, val);
        /*}*/
    }
    /* Write data image */
    for(j=0;data_img[j] != NULL;j++){
        unsigned int val;
        val = (data_img[j]->byte0<<17)|(data_img[j]->byte1<<9)|(data_img[j]->byte2);
        fprintf(file_desc, "%.7d %.6x\n", j + icf, val & 0xffffff);
    }
    fclose(file_desc);
}

/* Writes a table to a file. Each line has key and value, separated by a single space */
int write_table_to_file(table tab, char *filename, char *file_extension) {
	FILE *file_desc;
	char *full_filename = malloc_with_check(strlen(filename)+strlen(file_extension)+2);
	strcpy(full_filename, filename);
	strcat(full_filename, ".");
	strcat(full_filename, file_extension);
	file_desc = fopen(full_filename, "w");
	free(full_filename);
	while (tab != NULL) {
		fprintf(file_desc, "%s %.7d\n", tab->key, tab->value);
		tab = tab->next;
	}
	fclose(file_desc);
}