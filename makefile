all: fremen atreides
fremen.o: Fremen.c ServerInfo.h File.h Command.h
	gcc -c Fremen.c
file.o: File.c File.h ServerInfo.h
	gcc -c File.c
frame.o: Frame.c Frame.h
	gcc -c Frame.c
command.o: Command.c Command.h
	gcc -c Command.c
plotcreation.o: Plotcreation.c Plotcreation.h
	gcc -c Plotcreation.c
fremen: fremen.o file.o command.o plotcreation.o
	gcc Fremen.o File.o Command.o Plotcreation.o -o Fremen.exe
atreides.o: Atreides.c Atreides.h File.h Frame.h
	gcc -c Atreides.c
atreides: atreides.o file.o frame.o
	gcc atreides.o file.o frame.o -o Atreides.exe


