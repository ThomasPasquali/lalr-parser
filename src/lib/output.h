#ifndef LFC_LATEX_OUTPUT_H
#define LFC_LATEX_OUTPUT_H

#define PRE_FILENAME "src/files/pre.tex"
#define POST_FILENAME "src/files/post.tex"
#define OUT_TEX_FILENAME "graph.tex"
#define OUT_RAW_FILENAME "output.txt"

#define NON_FINAL_MARKER 'N'
#define FINAL_MARKER 'F'
#define EDGE_SYMBOL_STATE_SEP '>'
#define R_ITEM_ARROW "->"
#define R_ITEM_SEP ','
#define T_ITEM_ARROW "$\\rightarrow$"

#include "../structs/graph.h"
#include "../structs/automa.h"

void ouputLatexAutoma(Graph*, Automa* a);
void ouputRawAutoma(Graph*, Automa* a);

#endif //LFC_LATEX_OUTPUT_H
