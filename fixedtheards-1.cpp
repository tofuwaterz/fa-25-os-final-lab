#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Sums the first 100 numbers from the provided number
 * @param arg - the argument
 */ 
void* sum(void* arg)
{
	// Typecast the pointer
	int number = *(int*)arg; // *int instead of trying to store it in int
	
	// The sum
	int sum = 0;
	
	// Compute the sum
	for(int num = number; num < number + 100; ++num)
	{
		sum += num;
	}
	
	fprintf(stderr, "Sum = %d\n", sum); // declare sum here 
	return NULL;
}

int main()
{
	// An arbitrary number to start from
	int startNum = 10;
	
	// Create a thread
	pthread_t id; // thread id wasn't declared
	if(pthread_create(&id, NULL, sum, &startNum) < 0) // add & before startNum 
	{
		perror("pthread_create");
		exit(1);
	}
	pthread_join(id, NULL); // Are you trying to create an orphan process wait for the thread. Patience is key. - Note to self
	
	
	return 0;
}
