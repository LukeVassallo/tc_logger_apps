CC=gcc
AR=ar
ARFLAGS = -rcs
CFLAGS = -Wall -Werror -Wextra 
LDFLAGS = -L ./lib -lrt -ldl -lm
INCLUDES = -I ./includes/

LIB = libxlc.a

all : print_info library
library : ./lib/$(LIB)

./lib/$(LIB) : ./build/xlc.o
	@echo "Using the GNU archiver to create library"
	@echo "Creating the lib directory"
	@mkdir -p ./lib
	$(CROSS_COMPILE)$(AR) $(ARFLAGS) $@ $^ 

	@echo ""
	@echo "Library creation was successful. The library is located at ./lib/$(LIB)"

./build/%.o : ./src/%.c
	@echo "Creating the build directory"
	@mkdir -p ./build
	@echo "Compiling $< to $@"
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

print_info :
	@echo "Using compiler $(CROSS_COMPILE)$(CC)"	

clean: 
	@echo "Cleaning"
	@echo "Deleting the build directory."
	@rm -fvr ./lib
	@rm -fvr ./build

help: 
	@echo "make all : builds the library"
	@echo "make library : also builds the library"
	@echo "make clean : removes the build and lib directories."
