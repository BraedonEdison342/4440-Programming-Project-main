#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <time.h>

int arr[1000];
int count = 0;

float mean_result = 0.0f;
float std_result = 0.0f;
float median_result = 0.0f;
int max_result = 0;
int mode_result = 0;


// ---------------- THREADS --------------------- //

int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void* max_thread(void* arg) {
    max_result = arr[0];
    for (int i = 0; i < count; i++) {
        if (arr[i] > max_result) {
            max_result = arr[i];
        }
    }
    return NULL;
}

void* median_thread(void* arg) {
    // copy array 
    int* copy = malloc(count * sizeof(int));

    for (int i = 0; i < count; i++) {
        copy[i] = arr[i];
    }

    qsort(copy, count, sizeof(int), cmpfunc);

    if (count % 2 == 0) {
        median_result = (copy[count/2 - 1] + copy[count/2]) / 2.0f;
    } else {
        median_result = copy[count/2];
    }

    free(copy);

    return NULL;
}

void* mode_thread(void* arg) {
    int max_count = 0;

    for (int i = 0; i < count; i++) {
        int freq = 0;

        for (int j = 0; j < count; j++) {
            if (arr[j] == arr[i]) {
                freq++;
            }
        }

        if (freq > max_count) {
            max_count = freq;
            mode_result = arr[i];
        }
    }

    return NULL;
}


void* mean_thread(void* arg) {
    float total = 0.0f;

    for (int i = 0; i < count; i++) {
        total += arr[i];
    }

    mean_result = total / count;

    return NULL;
}

void* std_thread(void* arg) {
    float variance = 0.0f;

    for (int i = 0; i < count; i++) {
        float diff = arr[i] - mean_result;
        variance += diff * diff;
    }

    std_result = sqrtf(variance / (count - 1));

    return NULL;
}

// ---------------- THREADS --------------------- //


int main() {
    clock_t start = clock();
    //----------  FILE READ ------------
    FILE *fp = fopen("numbers.txt", "r");
    if (!fp) {
        printf("Error: could not open file.\n");
        return 1;
    }
    while (count < 1000 && fscanf(fp, "%d", &arr[count]) == 1) {
        count++;
    }
    fclose(fp);

    // ---------------- NO THREADS, CALL DIRECTLY ------------
    mean_thread(NULL);     // must run before std
    median_thread(NULL);
    max_thread(NULL);
    mode_thread(NULL);
    std_thread(NULL);      // depends on mean_result

    // ---------- PRINT RESULTS ---------------
    printf("Mean   = %.2f\n", mean_result);
    printf("Median = %.2f\n", median_result);
    printf("Max    = %d\n",   max_result);
    printf("Mode   = %d\n",   mode_result);
    printf("StdDev = %.4f\n", std_result);
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC; // CHAT GPT CLOCK EXAMPLE
    printf("Time taken = %.6f seconds\n", time_taken);
    return 0;
}
