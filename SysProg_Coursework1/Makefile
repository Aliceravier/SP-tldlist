tldmonitor: tldmonitor.o date.o tldlist.o
	clang -Wall -Werror -o tldmonitor tldmonitor.o date.o tldlist.o

date.o: date.h date.c
	clang -Wall -Werror -o date.o -c date.c

tldlist.o: tldlist.h tldlist.c
	clang -Wall -Werror -o tldlist.o -c tldlist.c

tldmonitor.o: tldmonitor.c date.h tldlist.h
	clang -Wall -Werror -o tldmonitor.o -c tldmonitor.c

clean:
	rm -f *.o tldmonitor

debug:
	clang tldmonitor.c tldlist.c date.c -g -o tldmonitor
	
gdb:
	gdb --tui --args ./tldmonitor 01/01/2000 01/01/2003 < small.txt
