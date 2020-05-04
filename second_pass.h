#include "globals.h"
#include "table.h"

bool process_line_spass(char *line, long *ic, machine_word **code_img, table *symbol_table);

bool add_symbols_to_code(char *line, long *ic, machine_word **code_img, table *symbol_table);