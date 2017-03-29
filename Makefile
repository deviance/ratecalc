all: librate
	gcc -std=gnu99 -Wall -Wextra -Wpedantic -static -o ratecalc -L$(shell pwd) ratecalc.c -lrate

librate: rate_limit.c rate.c expr.c
	gcc -std=gnu99 -Wall -Wextra -Wpedantic -c rate_limit.c rate.c expr.c
	ar rcs librate.a rate_limit.o rate.o expr.o

clean:
	rm -f *.o *.a ratecalc
