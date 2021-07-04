CC ?= gcc
CFLAGS = -Wall -Werror -Wextra
BIN=logger_simple

all : build/$(BIN)

build/$(BIN) : build/main.o
	@echo "Linking and generating the binary executable \"$(BIN)\""
	$(CC) $< -o $@ $(LDFLAGS)

	@echo ""
	@echo "The binary file is located at ./build/$(BIN)"

./build/%.o : ./src/%.c
	mkdir -p ./build
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean: 
	@echo "Cleaning"
	@echo "Deleting the build directory."
	@rm -fr ./build
