#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of singleprecision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* Pairwise summation implementation for an arbitrary float array. */
float my_fsum(FloatArray *floats) {
    /* N represents the base case length for naive summation */
    int N = 5;
    float sum;
    int i, m;

    /* Base case where we perform naive summation of array. */
    if (floats->count <= N) {
        sum = 0;
        for (i = 0; i < floats->count; i++) {
            sum += floats->values[i];
        }
    }
    /* Recursive case where we half our array and recursively add
       the two halves. */
    else {

      /* Calculating the midway point of the numbers. */
      m = floats->count/2;

      /* These will hold the first and second halves. */
      FloatArray first;
      FloatArray second;

      /* Float array of first half: [0, m-1] */
      first.values = floats->values;
      first.count = m;

      /* Float array of second half: [m, n-1] */
      /* Start array at mth value of original array (move the pointer) */
      second.values = floats->values + m;
      second.count = floats->count - m;

      /* Call pairwise summation on both halves */
      /* Function takes in pointer so send the addreses */
      sum =  my_fsum(&first) + my_fsum(&second);

    }

    return sum;
}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floatingpoint value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);


    printf("My sum:  %e\n", my_sum);


    return 0;
}
