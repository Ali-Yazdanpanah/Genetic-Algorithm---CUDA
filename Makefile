all: GA-serial 

GA-serial: ga.c
	gcc -pg ga.c -o ga-serial

clean:
	rm ga-serial
	