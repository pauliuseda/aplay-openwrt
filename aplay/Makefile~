# build helloworld executable when user executes "make"
serv: serv.o
	$(CC) $(LDFLAGS) serv.o -o serv -lasound -lm 
clie.o: serv.c
	$(CC) $(CFLAGS) -c serv.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o serv
