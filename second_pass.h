#include "globals.h"
#include "table.h"

bool process_line_spass(char *line, table *ent_table, table *ext_references, table code_table, long *ic, table ext_table,
                        table data_table, machine_word **code_img);

bool add_symbols_to_code(char *line, long *ic, machine_word **code_img, table data_table, table code_table, table ext_table, table *ext_references);