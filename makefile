all: fremen atreides
fremen.o: Fremen.c ServerInfo.h File.h Command.h
	gcc -c Fremen.c
file.o: File.c File.h ServerInfo.h
	gcc -c File.c
command.o: Command.c Command.h
	gcc -c Command.c
fremen: fremen.o file.o command.o 
	gcc Fremen.o File.o Command.o -o Fremen.exe
atreides.o: Atreides.c Atreides.h File.h
	gcc -c Atreides.c
atreides: atreides.o
	gcc atreides.o file.o -o Atreides.exe


