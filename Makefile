OBJS = main.c lz78.c lz78.h
CFLAGS = -c 

EXE = tp01


tp01: $(OBJS)
	gcc -pg -o tp01 $(OBJS)


	
clean:
	rm -f $(EXE) gmon.out
