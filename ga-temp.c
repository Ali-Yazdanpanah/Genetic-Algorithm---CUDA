#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GENES "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define TARGET "HELLO WORLD"
#define RAND_MAX 53

typedef struct
{
    char chromosome[11];
    int fitness;
} individual;

typedef struct
{
    individual *population;
    unsigned int population_size;
    unsigned int generation;
} generation;

// A utility function to swap two elements 
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than the pivot 
        if (arr[j] < pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 



int cal_fitness(individual individual)
{
    int len = strlen(TARGET);
    int fitness = 0;
    for (int i = 0; i < len; len++)
    {
        if (TARGET[i] != individual.chromosome[i])
        {
            fitness += 1;
        }
    }
    return fitness;
};

char mutate()
{
    return GENES[rand() % RAND_MAX];
}

double rand_double()
{
    return rand() / (double)1;
}

individual mate(individual male, individual female)
{
    int len = strlen(male.chromosome);
    individual child;
    double prob;
    for (int i = 0; i < len; i++)
    {
        prob = rand_double();
        if (prob < 0.45)
        {
            child.chromosome[i] = male.chromosome[i];
        }
        if (prob < 0.90)
        {
            child.chromosome[i] = female.chromosome[i];
        }
        else
        {
            child.chromosome[i] = mutate();
        }
    }
    return child;
}

int random_chromosome(generation *generation)
{
    time_t seed;
    /* Intializes random number generator */
    srand((unsigned)time(&seed));
    for (int i = 0; i < generation->population_size; i++)
    {
        for (int j = 0; j < strlen(generation->population[i].chromosome); j++)
        {
            generation->population[i].chromosome[j] = mutate();
        }
    }
    return 1;
}

void quickSort(generation * generation, int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(generation->population, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

void initialize_population(int chromosome_length, int population_size, char generation[][chromosome_length])
{
    int i, j;
    int len = strlen(TARGET);
    for(i = 0; i<population_size; i++){
        for(j = 0; j<len ; j++){
            generation[i][j] = mutate();
        }
    }
}

int individual_fitness_calculation(int index ,char chromosome[], int fitness[])
{
    int len = strlen(TARGET);
    int population = len(fitness);
    int new_fitness;

    for (int i = 0; i < len; i++)
    {
        if (chromosome[i] != TARGET[i])
        {
            new_fitness += 1;
        }
    }
    fitness[index] = new_fitness;
    return new_fitness;
}

int generation_fitness_calculation(int chromosome_length, int population_size, char generation[][chromosome_length], int fitness[])
{
    int result, i;
    int len = strlen(TARGET);
    for (i = 0; i < population_size; i++)
    {
        result = individual_fitness_calculation(generation[i], fitness);
        if (result == 0)
        {
            return i;
            break;
        }
    }
    return -1;
}

int sort_generation(generation * generation){
    
    return 1;
}

int generate_next_generation(generation * generation){
    sort_generation(generation);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Correct way to execute this program is:\n");
        printf("./ga-serial PopulationSize.\n");
        return 1;
    }
    int population_size = atoi(argv[1]), result;
    time_t seed;
    int len = strlen(TARGET);
    /* Intializes random number generator */
    srand((unsigned)time(&seed));
    char generation[population_size][len];
    initialize_population(len, population_size, generation);
    while (1)
    {
        if (result != -1)
        {
            //target found
            printf("target found (%d'th member of population of %d'th generation\n", result, generation->generation);
            return 1;
        }
        else
        {
            generate_next_generation(generation);
        }
    }
    result = generation_fitness_calculation(generation);

    int temp = strlen(TARGET);
    printf("%c\n", mutate());
}
