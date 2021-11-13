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
* Il programma si preoccupa di eliminare gli spazi nell'input, tutti gli altri caratteri saranno identificati come parte del vocabolario;
* Lo START SYMBOL è identificato dal driver della prima produzione;
* I caratteri A-Z sono interpretati come non-terminali mentre i caratteri a-z come terminali.

#### Vincoli sul file di input
* Il programma utilizza il tipo "char" per codificare i caratteri;
* Il simbolo "~" è utilizzato per identificare la produzione canonica S' -> S e non può essere quindi utilizzato come simbolo del vocabolario.

## Output

