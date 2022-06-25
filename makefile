all: fremen atreides
fremen.o: Fremen.c ServerInfo.h File.h Command.h
	gcc -c Fremen.c -Wall -Wextra
file.o: File.c File.h ServerInfo.h
	gcc -c File.c -Wall -Wextra
userManagement.o: UserManagement.c UserManagement.h
	gcc -c UserManagement.c -Wall -Wextra
frame.o: Frame.c Frame.h
	gcc -c Frame.c -Wall -Wextra
command.o: Command.c Command.h ServerInfo.h Frame.h
	gcc -c Command.c -Wall -Wextra
plotcreation.o: Plotcreation.c Plotcreation.h
	gcc -c Plotcreation.c -Wall -Wextra
fremen: fremen.o file.o command.o plotcreation.o frame.o
	gcc Fremen.o File.o Frame.o Command.o Plotcreation.o -o Fremen.exe -Wall -Wextra
atreides.o: Atreides.c Atreides.h File.h Frame.h Connection.h UserManagement.h Plotcreation.h
	gcc -c Atreides.c -Wall -Wextra
atreides: atreides.o file.o frame.o userManagement.o plotcreation.o
	gcc Atreides.o File.o Frame.o UserManagement.o Plotcreation.o -o Atreides.exe -Wall -Wextra -lpthread

