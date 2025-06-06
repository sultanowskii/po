TARGET_COMPILER    := poc
TARGET_INTERPRETER := poi

SRC_DIR     := src
GEN_DIR     := $(SRC_DIR)/gen
GRAMMAR_DIR := $(SRC_DIR)/grammar

SRC_COMPILER_DIR    := $(SRC_DIR)/compiler
SRC_INTERPRETER_DIR := $(SRC_DIR)/interp

PARSER_DEFINITION := $(GRAMMAR_DIR)/po.y
PARSER_GEN_SRC    := $(GEN_DIR)/po.tab.c
PARSER_GEN_HDR    := $(GEN_DIR)/po.tab.h
PARSER_GEN_SRCS   := $(PARSER_GEN_SRC) $(PARSER_GEN_HDR)

LEXER_DEFINITION := $(GRAMMAR_DIR)/po.l
LEXER_GEN_SRC    := $(GEN_DIR)/po.yy.c
LEXER_GEN_SRCS   := $(LEXER_GEN_SRC)

SRCS := $(shell find $(SRC_DIR)/ -type f -name '*.c' | grep -v "^$(SRC_INTERPRETER_DIR)/" | grep -v "^$(SRC_COMPILER_DIR)") $(PARSER_GEN_SRC) $(LEXER_GEN_SRC)
HDRS := $(shell find $(SRC_DIR)/ -type f -name '*.h' | grep -v "^$(SRC_INTERPRETER_DIR)/" | grep -v "^$(SRC_COMPILER_DIR)") $(PARSER_GEN_HDR) $(LEXER_GEN_HDR)
OBJS := $(SRCS:.c=.o)

COMPILER_SRCS := $(shell find $(SRC_COMPILER_DIR)/ -type f -name '*.c')
COMPILER_HDRS := $(shell find $(SRC_COMPILER_DIR)/ -type f -name '*.h')
COMPILER_OBJS := $(COMPILER_SRCS:.c=.o)

INTERPRETER_SRCS := $(shell find $(SRC_INTERPRETER_DIR)/ -type f -name '*.c')
INTERPRETER_HDRS := $(shell find $(SRC_INTERPRETER_DIR)/ -type f -name '*.h')
INTERPRETER_OBJS := $(INTERPRETER_SRCS:.c=.o)

FILES_TO_LINT := $(shell find $(SRC_DIR)/ -type f -name '*.c' -o -name '*.h' | grep -v "^$(GEN_DIR)/")

.PHONY: all
all: $(TARGET_INTERPRETER)

$(TARGET_COMPILER): $(OBJS) $(COMPILER_OBJS)
	$(CC) -I$(SRC_DIR) -lfl -o $(TARGET_COMPILER) $^

$(TARGET_INTERPRETER): $(OBJS) $(INTERPRETER_OBJS)
	$(CC) -I$(SRC_DIR) -lfl -o $(TARGET_INTERPRETER) $^

%.o: %.c
	$(CC) -I$(SRC_DIR) -lfl -c -o $@ $<

$(PARSER_GEN_SRCS): $(PARSER_DEFINITION)
	bison -Wcounterexamples --output=$(PARSER_GEN_SRC) --header=$(PARSER_GEN_HDR) $<

$(LEXER_GEN_SRCS): $(LEXER_DEFINITION)
	flex --outfile=$(LEXER_GEN_SRC) $<

.PHONY: graph
# Uses graphviz tool (dot)
graph:
	bison --graph src/grammar/po.y
	rm po.tab.c
	dot po.gv -T png -o po.png

.PHONY: clang-format
clang-format:
	clang-format -i $(FILES_TO_LINT)

.PHONY: clean
clean:
	rm -f $(TARGET_COMPILER)
	rm -f $(TARGET_INTERPRETER)
	rm -f $(OBJS) $(COMPILER_OBJS) $(INTERPRETER_OBJS)
	rm -f *.elf
	rm -f *.gv
	rm -f *.png
	rm -f *.out
