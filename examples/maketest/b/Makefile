# Build a static lib B.

libb.a: b.o
	ar rcs $@ $^  # Use AR to make the static library. rcs are options for ar.
	# $@ is the target, $^ are the prereqs

b.o: b.c
	gcc $^ -c -o $@  # Use GCC to build the intermediary object file b.o

clean:
	rm -f *.o
