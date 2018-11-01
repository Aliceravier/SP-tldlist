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

run:
	./tldmonitor 01/01/2000 01/01/2004 small.txt

run_small:
	./tldmonitor 01/01/2000 01/01/2004 small.txt

run_large:
	./tldmonitor 01/01/1000 01/01/3000 large.txt


debug:
	clang tldmonitor.c tldlist.c date.c -g -o tldmonitor
	gdb --tui ./tldmonitor
#r date date file
	
gdb:
	r 01/01/2000 01/01/2004 small.txt
