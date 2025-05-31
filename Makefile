TARGET := poc

GEN_DIR := src/gen

all:
	bison -Wcounterexamples --output=src/gen/po.tab.c --header=src/gen/po.tab.h src/grammar/po.y
	flex -o src/gen/po.yy.c src/grammar/po.l
	gcc -Isrc -lfl $(GEN_DIR)/*.c src/compiler/ast/*.c -o $(TARGET)

# Uses graphviz tool (dot)
graph:
	bison --graph src/grammar/po.y
	rm po.tab.c
	dot po.gv -T png -o po.png

clean:
	rm $(TARGET)
	rm *.gv
	rm *.png
	rm *.out
