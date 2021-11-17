# Generatore automa caratteristico per grammatiche LALR(1)

## Compilazione

Per compilare il programma è sufficente chiamare gcc passando tutti i file "*.c" e "*.h" in "src" e sottocartelle, comoda alternativa è utilizzare il Makefile fornito.
```console
# wd: src

# Manual
gcc main.c alg/lalr.c ...... -o main 

# Makefile
make
```
Questo genererà il file eseguibile "main".

## Input

Per poter generare l'automa si parte da una grammatica che il programma ha bisogno di conoscere.

Per semplicità implementativa ho fatto le seguenti scelte:
* V = {\[A-Za-z\]} U VALID_SYMBOLS (lib/utils.h);
* T = {\[a-z\]} U VALID_SYMBOLS (lib/utils.h);
* S: lo START SYMBOL è identificato dal driver della prima produzione passata in input;
* P: identificato nell'input.

Nota: Qualora si volesse modificare tali assunzioni è necessario modificare le funzioni e le costanti in "lib/utils.c" e "lib/utils.h", per avere simboli composti da più lettere è necessario rivedere buona parte del programma modificando le strutture dati e creando apposite funzioni di confronto e riconoscimento: nulla di che ma ho preferito la semplicità su questo aspetto nella mia implementazione.

### File di input

Il programma si aspetta di ricevere in input su stdin uno stream di caratteri che identifica un insieme di produzioni.

Esempio:
```console
S -> L=R | R
L -> *R | i
R -> L
```
#### Note e vincoli
* Il programma si preoccupa di eliminare gli spazi nell'input;
* Il separatore tra driver e body delle production è identificato da INPUT_DRIVER_BODY_SEP in "lib/inputParser.h" (default "`->`");
* Tutti i simboli del vocabolario sono caratteri ASCII stampabili (char standard di C), i caratteri UTF-16/32 potrebbero dare problemi;
* Il simbolo S1 in "lib/utils.h" (default "~") è utilizzato per identificare S' nella produzione canonica S' -> S, non può essere quindi utilizzato come simbolo del vocabolario;
* Il simbolo EPSILON in "lib/utils.h" (default "#") è utilizzato per identificare la epsilon,non può essere quindi utilizzato come simbolo del vocabolario;
* Il simbolo EOW in "lib/utils.h" (default "$") è utilizzato per identificare "fine parola".

## Utilizzo

Come già detto in precedenza, il programma aspetta su stdin una lista di produzioni che si attengano ai vincoli espressi poco fa.

In caso di chiamata errata al programma, riporto l'output fornito: 
```console
Usage: ./<executable> <flags> < <input> OR ./<executable> <flags>
Where:
    Executable: executable filename (default main);
    Flags: if empty all flags ("rtc") are added otherwise a string composed by one or more of the following chars:
            'r' for generating raw output in output.txt file;
            't' for generating graph.tex file;
            'c' for console debug output.
    Input: input stream containing the definition of the grammar (usually a filename).
i.e. ./main tc < inputs/5 OR ./main r < inputs/1
``` 

## Output

I flag 'r' e 't' permettono di generare rispettivamente i file OUT_RAW_FILENAME (default "output.txt") OUT_TEX_FILENAME (default "graph.tex") in "lib/output.h".

**L'automa in output sarà del tipo LR(1) merged.**

*Con il flag 'c' sarà possibile visualizzare a console anche l'automa LR(1) da cui deriva.*

### Raw output

Formato del file (# = numero):
```console
<tot # of states>\n
foreach state
<state #> <NON_FINAL_MARKER|FINAL_MARKER>
    {
    foreach transition 
    <transition symbol>EDGE_SYMBOL_STATE_SEP<target state> 
    }\n
foreach final state
<state #> 
    {
    foreach reducing item (if more than one separated by R_ITEM_SEP)
    <item production driver>R_ITEM_ARROW<item production body> <item lookahead set (no need for separator since symbols are supposed to besingle char)> 
    }\n
\n
<start symbol> <start state #>
```
Le costanti in maiuscolo sono modificabili in "lib/output.h".

Esempio di output.txt:
```console
10
0 N S>1 a>2
1 F
2 N A>3 b>4
3 N B>5 b>6 d>7
4 F
5 N e>8
6 N c>9
7 F
8 F
9 F
1 ~->S $
4 A->b bd
7 B->d e
8 S->aABe $
9 A->Abc bd

S 0
```

### LaTEX output

Sarà generato un file "graph.tex" composto da:

* Il contenuto del file PRE_FILENAME in "lib/output.h" (default "src/files/pre.tex");
* L'output generato dal programma;
* Il contenuto del file POST_FILENAME in "lib/output.h" (default "src/files/post.tex").

*La visualizzazione del grafo purtroppo non è sempre perfetta...*

*Se un grafo risulta incomprensibile si può rieseguire il programma, il nuovo grafo sarà leggermente diverso e, forse, più guardabile.*

Uno dei vari modi per visualizzare il grafo è generare un file .pdf con il comando:
```console
pdflatex graph.tex
```

## Riguardo il funzionamento del programma