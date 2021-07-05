CC = gcc
CFLAGS = -Wall -Werror -Wextra
BIN = parser_simple

all : print_info build/$(BIN)

build/$(BIN) : build/main.o
	@echo "Linking and generating the binary executable \"$(BIN)\""
	$(CROSS_COMPILE)$(CC) $< -o $@ $(LDFLAGS)

	@echo ""
	@echo "The binary file is located at ./build/$(BIN)"

./build/%.o : ./src/%.c
	mkdir -p ./build
	@echo "Compiling $< to $@"
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

print_info :
	@echo "Using compiler $(CROSS_COMPILE)$(CC)"	
	
clean: 
	@echo "Cleaning"
	@echo "Deleting the build directory."
	@rm -fvr ./build
