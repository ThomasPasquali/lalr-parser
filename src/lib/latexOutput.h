#ifndef LFC_LATEX_OUTPUT_H
#define LFC_LATEX_OUTPUT_H

#define PRE_FILENAME "src/files/pre.tex"
#define POST_FILENAME "src/files/post.tex"
#define OUT_FILENAME "graph.tex"

#include "../structs/automa.h"

void ouputLatexAutoma(Automa*);

#endif //LFC_LATEX_OUTPUT_H
