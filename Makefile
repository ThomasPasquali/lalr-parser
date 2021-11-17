SRC=$(shell find ./src -regex '.*\.\(c\|h\)')
CFLAGS=-Wall -Wextra 
LDFLAGS=

main: $(SRC)
	@ gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)