SRC=$(shell find ./src -regex '.*\.\(c\|h\)')

main: $(SRC)
	@ gcc -o $@ $^