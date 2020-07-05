all: ga-serial ga-parallel

ga-serial: ga.c
	gcc -pg ga.c -o ga-serial

ga-parallel: ga.cu
	nvcc -o ga-parallel ga.cu

clean:
	rm ga-serial ga-parallel
