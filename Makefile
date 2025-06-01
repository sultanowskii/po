TARGET := poc

SRC_DIR     := src
GEN_DIR     := $(SRC_DIR)/gen
GRAMMAR_DIR := $(SRC_DIR)/grammar

PARSER_DEFINITION := $(GRAMMAR_DIR)/po.y
PARSER_GEN_SRC    := $(GEN_DIR)/po.tab.c
PARSER_GEN_HDR    := $(GEN_DIR)/po.tab.h
PARSER_GEN_SRCS   := $(PARSER_GEN_SRC) $(PARSER_GEN_HDR)

LEXER_DEFINITION := $(GRAMMAR_DIR)/po.l
LEXER_GEN_SRC    := $(GEN_DIR)/po.yy.c
LEXER_GEN_SRCS   := $(LEXER_GEN_SRC)

SRCS := $(shell find $(SRC_DIR)/ -type f -name '*.c') $(PARSER_GEN_SRCS) $(LEXER_GEN_SRCS)
HDRS := $(shell find $(SRC_DIR)/ -type f -name '*.h')
OBJS := $(SRCS:.c=.o)

FILES_TO_LINT := $(shell find $(SRC_DIR)/ -type f -name '*.c' -o -name '*.h' | grep -v "^$(GEN_DIR)/")

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -I$(SRC_DIR) -lfl -o $(TARGET) $^

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
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f *.elf
	rm -f *.gv
	rm -f *.png
	rm -f *.out
