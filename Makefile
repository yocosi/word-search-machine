#
# Link
#
CFLAGS = -Wall -g
bin/search_machine: ./obj/file.o ./obj/hash.o ./obj/main.o
	gcc $(CFLAGS) -o bin/search_machine ./obj/file.o ./obj/hash.o ./obj/main.o

#
# Objets of TP Search MAchine
#
obj/file.o: ./src/file.c
	gcc $(CFLAGS) -I./include -c src/file.c -o obj/file.o
obj/hash.o: ./src/hash.c
	gcc $(CFLAGS) -I./include -c src/hash.c -o obj/hash.o
obj/main.o: ./src/main.c
	gcc $(CFLAGS) -I./include -c src/main.c -o obj/main.o

#
# Search machine creation in pdf
#
pdf: ./src/file.c ./src/hash.c ./src/main.c ./include/types.h ./include/functions.h
	a2ps -o search_machine.ps ./include/types.h ./include/functions.h ./src/file.c ./src/hash.c ./src/main.c
	ps2pdf search_machine.ps
	rm search_machine.ps

#
# Remove files
#
clean :
	rm ./obj/*.o ./bin/search_machine

#
# Test the functionality
#
run :
	./bin/search_machine
