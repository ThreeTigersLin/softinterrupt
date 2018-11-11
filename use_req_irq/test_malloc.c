
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#include <GLFW/glfw3.h>

#define ARRAY_NUM (1000000 * 1000 * 7LL) // GIG
int main(int argc, char *argv[]) {

//    if(!glfwInit()) {
//        exit(EXIT_FAILURE);
//    }

    uint64_t *array = malloc(sizeof(uint64_t) * ARRAY_NUM);

//    double time_start = glfwGetTime();
    for(uint64_t i = 0; i < ARRAY_NUM; ++i) {
        *(array+i) = 0xff;
	if ((i) %  (1024*1024*1024) == 0){
	    printf("consume: %llu\n", i/1024/1024/1024);
	    sleep(5);
	}
		
    }
//    double time_end = glfwGetTime();

    free(array);
//    glfwTerminate();

//    double performance = ((ARRAY_NUM * sizeof(uint64_t))/1000000) / (time_end - time_start);
//    printf("Done in %f\n", (time_end - time_start));
//    printf("Performance: %f MB/s\n", performance);

//    exit(EXIT_SUCCESS);
}
