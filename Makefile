all: ga-serial ga-parallel

ga-serial: ga.c
	gcc -pg ga.c -o ga-serial

clean:
	rm ga-serial ga-parallel

ga-parallel: ga.cu
	gcc -pg ga.cu -o ga-parallel	