# build helloworld executable when user executes "make"
aplay: aplay.o
	$(CC) $(LDFLAGS) aplay.o -o aplay -lasound -lm 
clie.o: aplay.c
	$(CC) $(CFLAGS) -c aplay.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o aplay
