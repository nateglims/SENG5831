TARGET=a
INCLUDE=../b

$(TARGET): a.o
	gcc $^ -o $@ -I$(INCLUDE) -L$(INCLUDE) -lb

a.o: a.c
	gcc $^ -c -o $@ -I$(INCLUDE) # Compile a.c ($^ means all prereqs) into an
	# intermediary (-c) file called a.o (-o is output, $@ is target name). Include
	# some headers required to compile a (-I($INCLUDE))

clean:
	rm -f *.o
