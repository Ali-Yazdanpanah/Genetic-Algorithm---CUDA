
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define O_SPARTA 0x01

#define CHARMAP "abcdefghijklmnopqrstuvwxyz" \
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				" "

#define RANDBETWEEN(A, B) A + rand() / (RAND_MAX / (B - A))
#define CHANCE(A) rand() < A *RAND_MAX

static char *target = "Hello world";
static size_t el_sz;
static size_t total_sz;
static char options = 0;
static unsigned int pop_size = 2048;
static unsigned int challengers = 3;
static float elitism = .1;
static float mutation = .25;

static char
rndchr(char *map)
{
	return *(map + RANDBETWEEN(0, strlen(map)));
}

static char
randchar()
{
	return rndchr(CHARMAP);
}

static char *
rndstr(char *map, size_t strsize)
{
	char *result = malloc(strsize);
	size_t i;

	for (i = 0; i < strsize; i++)
	{
		*(result + i) = rndchr(map);
	};

	return result;
}

static int
fitness(char *str, char *gauge, size_t n)
{
	int i;
	int result = 0;

	for (i = n - 1; i >= 0; i--)
	{
		result += abs(str[i] - gauge[i]);
	}

	return result;
}

static int
_fitness(char *str)
{
	return fitness(str, target, el_sz);
}

static int
fit_cmp(const void *el1, const void *el2)
{
	int a = _fitness((char *)el1);
	int b = _fitness((char *)el2);

	if (a > b)
		return 1;
	if (a < b)
		return -1;
	return 0;
}

static void
mutate(char *p)
{
	*(p + RANDBETWEEN(0, el_sz)) = randchar();
}

static char *
rnd_el(char *p)
{
	unsigned int top = pop_size;

	if ((options & O_SPARTA) == O_SPARTA)
	{
		top = pop_size * elitism;
	}

	return p + el_sz * (int)(RANDBETWEEN(0, top));
}

static char *
trnmnt(char *p)
{
	size_t i;
	char *winner = rnd_el(p);
	char *challenger;
	int f1 = _fitness(winner);
	int f2;

	for (i = challengers; i > 0; i--)
	{
		challenger = rnd_el(p);
		f2 = _fitness(challenger);
		if (f2 < f1)
		{
			f1 = f2;
			winner = challenger;
		}
	}

	return winner;
}

static void
mate(char *p, char *buffer)
{
	char *a, *b;
	size_t i, pivot;
	size_t skip = (size_t)(elitism * pop_size) * el_sz;
	memcpy(buffer, p, total_sz);

	for (i = skip; i <= total_sz - el_sz; i += el_sz)
	{
		a = trnmnt(p);
		b = trnmnt(p);
		pivot = RANDBETWEEN(0, el_sz);

		strncpy(buffer + i, a, el_sz);
		strncpy(buffer + i, b, pivot);

		if (CHANCE(mutation))
		{
			mutate(buffer + i);
		}

		if (i < total_sz - el_sz)
		{
			i += el_sz;
			strncpy(buffer + i, b, el_sz);
			strncpy(buffer + i, a, pivot);
			if (CHANCE(mutation))
			{
				mutate(buffer + i);
			}
		}
	}

	memcpy(p, buffer, total_sz);
}

static void
run_tests(void)
{
	assert(000 == fitness("Hello world", "Hello world", 11));
	printf("Tests passed.\n\n");
}

static void
print_usage(char *self)
{
	printf("Usage: %s [-t] [-s] [-h] [-p SIZE] [-c COUNT] [-e RATIO] [-m RATIO] [-i STRING]\n", self);
	printf("	-t:		run tests\n");
	printf("	-s:		Sparta! mode (Only elite can mate)\n");
	printf("	-h:		Show this help\n");
	printf("	-p SIZE:	Population size\n");
	printf("	-c COUNT:	Challengers count for mate tournament\n");
	printf("	-e RATIO:	Elitism ratio\n");
	printf("	-m RATIO:	Mutation ratio\n");
	printf("	-i STRING:	search this instead of \"Hello, World!\"\n");
}

static void
check_params()
{
	if ((options & O_SPARTA) == O_SPARTA && ((int)(pop_size * elitism) == 0))
	{
		printf("You have not enough spartans.\n");
		exit(1);
	}
}

void swap(int a, int b, int *fitness, char *p, int el_sz)
{
	int temp = fitness[a];
	fitness[a] = fitness[b];
	fitness[b] = temp;
	char tempchar;
	for (int i = 0; i < el_sz; i++)
	{
		tempchar = p[a + i];
		p[a + i] = p[b + i];
		p[b + i] = tempchar;
	}
}

int partition(int *fitness, int low, int high, char *p, int el_sz)
{
	int pivot = fitness[high]; // pivot
	int i = (low - 1);		   // Index of smaller element
	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot
		if (fitness[j] < pivot)
		{
			i++; // increment index of smaller element
			swap(i, j, fitness, p, el_sz);
		}
	}
	swap(i + 1, high, fitness, p, el_sz);
	return (i + 1);
}

void quickSort(int *fitness, int low, int high, char *p, int el_sz)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now 
           at right place */
		int pi = partition(fitness, low, high, p, el_sz);
		// Separately sort elements before
		// partition and after partition
		quickSort(fitness, low, pi - 1, p, el_sz);
		quickSort(fitness, pi + 1, high, p, el_sz);
	}
}

void calculate_fitness(int *fitness, char *p, int el_sz, int total_sz, char *gauge)
{
	int result;
	int j = 0;
	for (int i = 0; i < total_sz; i += el_sz)
	{
		result = 0;
		for(int k = 0 ; k < el_sz; k++){
			result += abs(p[i+k] - gauge[k]);
		}
		fitness[j] = result;
		j++;
	}
}

int main(int argc, char **argv)
{
	struct timeval start, end;
	int i = 0;
	int bestfit = RAND_MAX;
	int opt;
	srand((unsigned int)time(NULL));
	while ((opt = getopt(argc, argv, "tshi:p:e:m:c:")) != -1)
	{
		switch (opt)
		{
		case 't':
			run_tests();
			break;
		case 's':
			options |= O_SPARTA;
			break;
		case 'i':
			target = optarg;
			break;
		case 'p':
			pop_size = atoi(optarg);
			break;
		case 'c':
			challengers = atoi(optarg);
			break;
		case 'e':
			elitism = atof(optarg);
			break;
		case 'm':
			mutation = atof(optarg);
			break;
		case 'h':
		default:
			print_usage(argv[0]);
			exit(1);
		}
	}

	check_params();

	el_sz = strlen(target);
	total_sz = pop_size * el_sz;
	char *p = rndstr(CHARMAP, total_sz);
	char *b = malloc(total_sz);
	int *fitness = malloc(pop_size*sizeof(int));
	gettimeofday(&start, NULL);
	while (bestfit)
	{
		calculate_fitness(fitness, p, el_sz, total_sz, target);
		quickSort(fitness, 0, pop_size-1, p, el_sz);
		i += 1;
		if (bestfit != fitness[0])
		{
			bestfit = fitness[0];
			if(bestfit == 0) printf("[%03d] result found \n", i);
		}
		mate(p, b);
	}
	gettimeofday(&end, NULL);
	double diffDouble =
		(end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
	printf("Execution took : %.4fms\n", diffDouble);
	free(p);
	free(b);

	return 0;
}
