CC=gcc
CFLAGS=-O3
DEPS = partticle.h BoundBox.h physics.h draw2D.h bitmap.h
OBJ = main.o particle.o BoundBox.o physics.o draw2D.o bitmap.o
LIBS = -lm 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
