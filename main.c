/*
 *  A simple program for the custom malloc. Only the most basic cases are tested,
 *  and it's encouraged to add more test cases to ensure the functions work correctly.
 *  'passed' indicates that the function's operation did not create fatal errors
 */

#include "mm_alloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int
main(int argc, char **argv)
{
    int *data, i;
    double *ddata;
    bool passed;

    printf("Basic malloc....");
    ddata = (double*) mm_malloc(sizeof(double));
    *ddata = 12345.6789;
    printf("%s\n",(*ddata)==12345.6789?"'passed'":"failed!");

    printf("Array malloc....");
    data = (int*) mm_malloc(1028*sizeof(int));
    for(i = 0; i < 1028; i++) data[i] = i;
    passed = true;
    for(i = 0; i < 1028; i++) passed &= (data[i]==i);
    printf("%s\n",passed?"'passed'":"failed!");

    printf("Basic free......");
    mm_free(ddata);
    printf("'passed'\n");

    printf("Array free......");
    mm_free(data);
    printf("'passed'\n");

    printf("Basic realloc...");
    ddata = (double*) mm_malloc(sizeof(double));
    *ddata = 12345.6789;
    double* old = ddata;
    ddata = (double*) mm_realloc(ddata,1000*sizeof(double));
    passed = ((old<ddata)&&((*ddata)==12345.6789));
    mm_free(ddata);
    printf("%s\n",passed?"'passed'":"failed!");

    return 0;
}
