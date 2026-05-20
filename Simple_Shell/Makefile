CC := gcc

Shell: Simple_shell.c main.c
	$(CC) $^ -o $@
	-./$@

.PHONY:clean
clean:
	-rm -f *.o Shell