parser_simple = simple_parser/build/parser
parser_production = parser/build/parser

libxlc = libxlc/lib/libxlc.a 

all : clean_all $(libxlc) $(parser_simple) $(parser_production)

help :
	@echo ""
	@echo " HELP "
	@echo "======"
	@echo ""
	@echo "usage: make [CROSS_COMPILE=...] all - removes all library and application files (clean all) then rebuilds."
	@echo "usage: make [CROSS_COMPILE=...] <project-name> - builds the specific project and if required dependent libraries."
	@echo "       valid project names include: 'parser_simple' and 'parser_production'"
	@echo ""
	@echo "make clean - cleans all application build files."
	@echo "make clean_libs - cleans all library files."
	@echo "make clean_all - cleans all application build files and library files."

parser_simple : $(parser_simple) 

parser_production : $(parser_production)

libxlc : $(libxlc)

$(parser_simple) : 
	@echo "Building $@."
	make -C ./simple_parser CROSS_COMPILE=$(CROSS_COMPILE)

$(libxlc) : 
	@echo "Building $@."
	make -C ./libxlc CROSS_COMPILE=$(CROSS_COMPILE)

$(parser_production) : $(libxlc)
	@echo "Building $@."
	make -C ./parser CROSS_COMPILE=$(CROSS_COMPILE)

clean :
	make -C ./simple_parser clean
	make -C ./parser clean

clean_libs : 
	make -C ./libxlc clean

clean_all : 
	make -C ./simple_parser clean
	make -C ./parser clean
	make -C ./libxlc clean

