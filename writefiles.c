//
// Created by yotam on 26/04/2020.
//

#include <stdio.h>
#include <string.h>
#include "code.h"
#include "table.h"

#include "writefiles.h"
int write_ob(machine_word **code_img, int icf, int dcf, char filename[]){
    int val,i,j;
    FILE *ob_file;
    char obname[80];
    strcpy(obname,filename);
    strcat(obname,".ob");
    ob_file = fopen(obname,"w");
    if( ob_file== NULL ){
        printf("couldn't create ob file");
        return TRUE;
    }
    fprintf(ob_file, "%.7d %d\n",icf-100, dcf);
    for(i=0;i<11;i++) {
        for(j=0;i<code_img[i]->length,i<11;i++,j++){
            if(code_img[i]->length != 0) {
                val = (code_img[i]->word.code->opcode << 18) | (code_img[i]->word.code->src_addressing << 16) |
                      (code_img[i]->word.code->src_register << 13) | (code_img[i]->word.code->dest_addressing << 11) |
                      (code_img[i]->word.code->dest_register << 8) | (code_img[i]->word.code->funct) << 3 |
                      (code_img[i]->word.code->ARE);
            }
            else{
                val = (code_img[i]->word.data->data<<3)|(code_img[i]->word.data->ARE);
            }
            fprintf(ob_file,"%d %.6x\n", i+100,val);
        }
    }
    fclose(ob_file);
}