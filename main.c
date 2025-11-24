#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

extern void imgCvtGrayInttoFloat(int height, int width, unsigned char* input, float* output);


void imgCvtGrayInttoFloat_C(int height, int width, unsigned char* input, float* output) {
    int size = height * width;
    int i;
    for (i = 0; i < size; i++) {
        output[i] = (float)input[i] / 255.0f;
    }
}

void performanceTest(int height, int width, int runs) {
    int size = height * width;
    double totalTimeASM = 0.0;
    double totalTimeC = 0.0;
    
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    printf("\n---------------------------------------\n");
    printf("Performance Test: %d x %d\n", width, height);
    printf("Running %d times..\n", runs);
    printf("---------------------------------------\n");

    //allocation
    unsigned char* input = (unsigned char*)malloc(size * sizeof(unsigned char));
    float* outputASM = (float*)malloc(size * sizeof(float));
    float* outputC = (float*)malloc(size * sizeof(float));

    if (!input || !outputASM || !outputC) {
        printf("Memory allocation failed!\n");
        return;
    }

	//random data
	int i;
    for (i = 0; i < size; i++) {
        input[i] = rand() % 256; 
    }

    // test C function
    int r;
    for (r = 0; r < runs; r++) {
        QueryPerformanceCounter(&start);
        imgCvtGrayInttoFloat_C(height, width, input, outputC);
        QueryPerformanceCounter(&end);
        totalTimeC += (double)(end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart;
    }

    // test asm function
    for (r = 0; r < runs; r++) {
        QueryPerformanceCounter(&start);
        imgCvtGrayInttoFloat(height, width, input, outputASM);
        QueryPerformanceCounter(&end);
        totalTimeASM += (double)(end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart;
    }

    // verify
    int errors = 0;
    for (i = 0; i < size; i++) {
        float diff = outputC[i] - outputASM[i];
        if (fabs(diff) > 0.0001f)	errors++;
    }

   printf("Errors: %d\n", errors);

    // time comparison
    double avgC = totalTimeC / runs;
    double avgASM = totalTimeASM / runs;

    printf("Avg Time C:   %.2f us\n", avgC);
    printf("Avg Time ASM: %.2f us\n", avgASM);

    // free memory
    free(input);
    free(outputASM);
    free(outputC);
}

int main() {
    srand((unsigned int)time(NULL));
    int i, j;
    int width, height, size, runs;
    int choice;

    while (1) {
        printf("\n--------------------------\n");
        printf("[0] - Exit\n");
        printf("[1] - Input manually\n");
        printf("[2] - Run performance test\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        if (choice == 1) {
            printf("\nEnter height and width: ");
            scanf("%d %d", &height, &width);
            size = height * width;

            // allocate
            unsigned char* input = (unsigned char*)malloc(size * sizeof(unsigned char));
            float* outputASM = (float*)malloc(size * sizeof(float));
            float* outputC = (float*)malloc(size * sizeof(float));

            // input data
            printf("Enter %d integer values (0-255):\n", size);
            for (i = 0; i < size; i++) {
                int val;
                scanf("%d", &val);
                input[i] = (unsigned char)val;
            }

            // run both
            imgCvtGrayInttoFloat(height, width, input, outputASM);
            imgCvtGrayInttoFloat_C(height, width, input, outputC);

            // display both outputs
            printf("\nAssembly Output\n");
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    int index = i * width + j;
                    printf("%.2f ", outputASM[index]);
                }
                printf("\n");
            }
            
            printf("\nC Output\n");
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    int index = i * width + j;
                    printf("%.2f ", outputC[index]);
                }
                printf("\n");
            }

            free(input);
            free(outputASM);
            free(outputC);
        }
        else if (choice == 2) {
            runs = 30;
            performanceTest(10, 10, runs);
            performanceTest(100, 100, runs);
            performanceTest(1000, 1000, runs);
        }
    }

    return 0;
}

