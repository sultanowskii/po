all:
	bison -Wcounterexamples --header=src/grammar/po.tab.h --output=src/grammar/po.tab.c src/grammar/po.y
	flex -o src/grammar/po.yy.c src/grammar/po.l
	gcc -Isrc -lfl src/grammar/*.c -o poc

# Uses graphviz tool (dot)
graph:
	bison --graph src/grammar/po.y
	rm po.tab.c
	dot po.gv -T png -o po.png
