#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
    FILE *fp = fopen("numbers.txt", "w");

    srand(time(NULL));  

    for (int i = 0; i < 1000; i++) {
        int num = rand() % 1000; 
        fprintf(fp, "%d\n", num);
    }

    fclose(fp);
    return 0;
}
