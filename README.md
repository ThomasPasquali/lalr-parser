# Generatore automa caratteristico per grammatiche LALR(1)

## Input

Compilazione ed esecuzione
```console
make && ./main < <input file path>
```
### File di input

Il programma si aspetta di ricevere in input su stdin uno stream di caratteri che identifica un insieme di produzioni. 
Esempio
```console
S -> AaB | b
A -> BcBaA | Ɛ
B -> Ɛ
```
#### Nota bene
* Il programma si preoccupa di eliminare gli spazi nell'input;
* Il vocabolario è composto dai caratteri \[A-Za-z\] più i caratteri in VALID_SYMBOLS (lib/utils.h);
* Lo START SYMBOL è identificato dal driver della prima produzione;
* I caratteri A-Z sono interpretati come non-terminali, tutti gli altri come terminali.

#### Vincoli sul file di input
* Il programma utilizza il tipo "char" per codificare i caratteri;
* Il simbolo "~" è utilizzato per identificare la produzione canonica S' -> S e non può essere quindi utilizzato come simbolo del vocabolario.

## Output

