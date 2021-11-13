SRC=$(shell find ./src -regex '.*\.\(c\|h\)')
CFLAGS=-Wall -Wextra `pkg-config --cflags icu-uc icu-io`
LDFLAGS=`pkg-config --libs icu-uc icu-io`

main: $(SRC)
	@ gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)