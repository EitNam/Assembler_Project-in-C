myProject: project.o macroHandeling.o firstPass.o secondPass.o
	gcc -ansi -Wall -g project.o macroHandeling.o firstPass.o secondPass.o -o myProject
project.o: project.c project.h
	gcc -c -ansi -Wall -g project.c
macroHandeling.o: macroHandeling.c project.h macro.h
	gcc -c -ansi -Wall -g macroHandeling.c
firstPass.o: firstPass.c project.h firstPass.h
	gcc -c -ansi -Wall -g firstPass.c
secondPass.o: secondPass.c project.h secondPass.h
	gcc -c -ansi -Wall -g secondPass.c
