#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    printf("\nLaunching sequential_min_max with parameters:\n");
    printf("seed: 3\narray_size: 10\n\n");
    
    int pid = fork();
    if (pid > 0) {
        printf("Done!\n");
        wait(NULL);
    }
    else if ( pid == 0 ) {
		execl("sequential_min_max", "./sequential_min_max",
		"3", "10", NULL);
	}
	else {
	    printf("\nProgram launching failed!\n");
	    exit(EXIT_FAILURE);
	}
    
    return EXIT_SUCCESS;
}
