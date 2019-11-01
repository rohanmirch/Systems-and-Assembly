#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "lists.h"
#include "realtime.h"


#define DEFAULT_MIN_N 1
#define DEFAULT_MAX_N 4000
#define DEFAULT_STEP_N 1
#define DEFAULT_REPS 3

#define DEFAULT_SEED 24242424


bool verify = false;


typedef uint64_t (*test_fn_t)(list_t *, int);


/*!
 * Verify that the list contains n elements, and the values should be 0..n-1
 * in increasing order.
 */
void sanity_check_list_contents(list_t *list, int n) {
    assert(list != NULL);
    assert(n > 0);

    int size = list_size(list);
    if (list_size(list) != n) {
        fprintf(stderr, "ERROR:  expected list size %d; found %d instead\n",
            n, size);
    }

    // Sanity-check the values in the list.
    list_iter_t *iter = list_iter(list);
    for (int i = 0; i < size; i++) {
        int value = list_iter_get(list, iter);
        if (value != i) {
            fprintf(stderr,
                "ERROR:  expected list[%d] == %d; found %d instead\n", i, i,
                value);
        }
        iter = list_iter_next(list, iter);
    }

    if (iter != NULL) {
        fprintf(stderr,
            "ERROR:  traversed list %d times but iter is still not NULL\n", n);
    }
}


/*!
 * Tests the performance of inserting N values into a list structure at
 * index 0.
 *
 * The provided list must be empty.
 * The value of n must be positive, but a value of 1 is probably not useful.
 */
uint64_t test_insert_0(list_t *list, int n) {
    assert(list != NULL);
    assert(list_size(list) == 0);
    assert(n > 0);

    uint64_t start = rdtsc();

    // Insert each value at index 0.
    for (int i = n - 1; i >= 0; i--)
        list_insert(list, 0, i);

    uint64_t end = rdtsc();

    if (verify)
        sanity_check_list_contents(list, n);

    return end - start;
}


/*!
 * Tests the performance of appending N values into a list structure.
 *
 * The provided list must be empty.
 * The value of n must be positive, but a value of 1 is probably not useful.
 */
uint64_t test_append(list_t *list, int n) {
    assert(list != NULL);
    assert(list_size(list) == 0);
    assert(n > 0);

    uint64_t start = rdtsc();

    // Append each value to the list.
    for (int i = 0; i < n; i++)
        list_append(list, i);

    uint64_t end = rdtsc();

    if (verify)
        sanity_check_list_contents(list, n);

    return end - start;
}


/*!
 * Tests the performance of traversing a list of N values.  The elements are
 * added in some random order.
 *
 * The provided list must be empty, and is loaded with the values 0..n-1.
 * The value of n must be positive, but a value of 1 is probably not useful.
 */
uint64_t test_iter(list_t *list, int n) {
    assert(list != NULL);
    assert(list_size(list) == 0);
    assert(n > 0);

    int *values = malloc(sizeof(int) * n);
    if (values == NULL) {
        fprintf(stderr, "ERROR:  Cannot allocate array of %d integers\n", n);
        abort();
    }

    // Put all values into the array, then shuffle it so it's in a random order.
    for (int i = 0; i < n; i++)
        values[i] = i;

    for (int i = 0; i < n - 1; i++) {
        int j = i + random() % (n - i);
        int tmp = values[i];
        values[i] = values[j];
        values[j] = tmp;
    }

    // Add the values to the list, then sort the list.
    for (int i = 0; i < n; i++)
        list_append(list, values[i]);

    list_sort(list);

    free(values);

    if (verify)
        sanity_check_list_contents(list, n);

    // Access each element of the list in random order.

    uint64_t start = rdtsc();

    list_iter_t *iter = list_iter(list);
    for (int i = 0; i < n; i++) {
        int value = list_iter_get(list, iter);
        if (value != i) {
            fprintf(stderr, "ERROR:  Expected value %d at index %d in list,"
                    " found %d instead\n", i, i, value);
            abort();
        }
        iter = list_iter_next(list, iter);
    }
    assert(iter == NULL);  // Should have reached the end of the sequence.

    uint64_t end = rdtsc();

    return end - start;
}


/*!
 * Runs the specified list performance test on a range of input values from
 * min_n to max_n (inclusive).  If reps > 1 then the test for each n will be
 * run multiple times, and the average will be calculated.
 */
void run_perf_test(const char *test_fn_name, test_fn_t test_fn, list_t *list,
    int min_n, int max_n, int step_n, int reps) {

    assert(min_n > 0);
    assert(max_n >= min_n);
    assert(reps >= 1);

    assert(test_fn_name != NULL);
    assert(test_fn != NULL);

    assert(list != NULL);

    fprintf(stderr, "Running \"%s\" test, n = [%d, %d], step = %d, reps = %d\n\n",
        test_fn_name, min_n, max_n, step_n, reps);

    printf("N\tTotal Time\tAverage CPE\n");
    for (int test = 0, n = min_n; n <= max_n; n += step_n) {
        test++;
        if (test % 100 == 0)
            fprintf(stderr, ".");

        // Perform the test a specific number of times, accumulating the total
        // time.  Then divide this by the number of repetitions to get the
        // average time taken.
        uint64_t test_time = 0;
        for (int iter = 0; iter < reps; iter++) {
            list_clear(list);

            test_time += test_fn(list, n);
        }
        test_time /= reps;

        double clocks_per_elem = (double) test_time / (double) n;

        printf("%d\t%" PRIu64 "\t%.2f\n", n, test_time, clocks_per_elem);
    }
    fprintf(stderr, " done\n\n");
}


/*! Prints out program usage details. */
void usage(const char *prog) {
    printf("usage: %s [options]\n", prog);
    printf("\tRuns the list performance tester with the specified options.\n\n");

    printf("-v | --verify\n");
    printf("\tVerify that the list contents are correct during performance testing.\n"
           "\tThis is done in a way that doesn't interfere with the performance\n"
           "\tmeasures, but it does slow down the benchmark.\n\n");

    printf("-S <seed> | --seed <seed>\n");
    printf("\tSpecifies the random number seed for testing.  Default is %d.\n\n",
        DEFAULT_SEED);

    printf("-t <test> | --test <test>\n");
    printf("\tSpecifies the performance test to run.  Available options are:\n");
    printf("\t\tinsert\tinsert elements at index 0 (default)\n");
    printf("\t\tappend\tappend elements\n");
    printf("\t\titer\titerate over elements of sequence in order\n\n");

    printf("-l <list> | --list <list>\n");
    printf("\tSpecifies the kind of list to use.  Available options are:\n");
    printf("\t\tarraylist\tarray-list (default)\n");
    printf("\t\tlinkedlist\tlinked list\n\n");

    printf("-m <min-n> | --min-n <min-n>\n");
    printf("\tThe minimum collection size to use during the performance benchmark.\n"
           "\tDefault is %d.\n\n", DEFAULT_MIN_N);

    printf("-n <max-n> | --max-n <max-n>\n");
    printf("\tThe maximum collection size to use during the performance benchmark.\n"
           "\tDefault is %d.\n\n", DEFAULT_MAX_N);

    printf("-s <step-n> | --step <step-n>\n");
    printf("\tThe amount to increase the collection size at each step of the\n"
           "\tperformance benchmark.  Default is %d.\n\n", DEFAULT_STEP_N);

    printf("-r <reps> | --reps <reps>\n");
    printf("\tSpecifies how many times to repeat the test for each collection size,\n"
           "\tin order to compute an average performance benchmark.  This can help\n"
           "\twhen performance results vary significantly.  Default is %d.\n\n",
           DEFAULT_REPS);
}


int main(int argc, char **argv) {
    int min_n = DEFAULT_MIN_N;
    int max_n = DEFAULT_MAX_N;
    int step_n = DEFAULT_STEP_N;

    int reps = DEFAULT_REPS;

    unsigned int seed = DEFAULT_SEED;

    char *test_fn_name = NULL;
    test_fn_t test_fn = test_insert_0;

    enum ListType { ARRAY_LIST, LINKED_LIST } list_type = ARRAY_LIST;

    int ch;
    while (true) {
        int option_index = 0;
        struct option longopts[] = {
            {"seed",   required_argument, 0, 'S'},
            {"verify", no_argument,       0, 'v'},
            {"list",   required_argument, 0, 'l'},
            {"test",   required_argument, 0, 't'},
            {"min-n",  required_argument, 0, 'm'},
            {"max-n",  required_argument, 0, 'n'},
            {"step-n", required_argument, 0, 's'},
            {"reps",   required_argument, 0, 'r'},
            {0,        0,                 0,  0 }
        };

        ch = getopt_long(argc, argv, "l:t:m:n:s:r:S:v", longopts, &option_index);
        if (ch == -1)
            break;

        switch (ch) {
        case 'l':
            if (strcmp(optarg, "arraylist") == 0) {
                list_type = ARRAY_LIST;
            }
            else if (strcmp(optarg, "linkedlist") == 0) {
                list_type = LINKED_LIST;
            }
            else {
                fprintf(stderr, "ERROR:  unrecognized list type \"%s\"\n", optarg);
                exit(1);
            }
            break;

        case 't':
            test_fn_name = strdup(optarg);
            if (strcmp(optarg, "insert") == 0) {
                test_fn = test_insert_0;
            }
            else if (strcmp(optarg, "append") == 0) {
                test_fn = test_append;
            }
            else if (strcmp(optarg, "iter") == 0) {
                test_fn = test_iter;
            }
            else {
                fprintf(stderr, "ERROR:  unrecognized test \"%s\"\n", optarg);
                exit(1);
            }
            break;

        case 'S':
            seed = atoi(optarg);
            break;

        case 'r':
            reps = atoi(optarg);
            if (reps <= 0) {
                fprintf(stderr, "ERROR:  number of repetitions must be positive\n");
                exit(1);
            }
            break;

        case 'm':
            min_n = atoi(optarg);
            if (min_n < 1) {
                fprintf(stderr, "ERROR:  min-n must be positive\n");
                exit(1);
            }
            break;

        case 'n':
            max_n = atoi(optarg);
            if (max_n < 1) {
                fprintf(stderr, "ERROR:  max-n must be positive\n");
                exit(1);
            }
            break;

        case 's':
            step_n = atoi(optarg);
            if (step_n < 1) {
                fprintf(stderr, "ERROR:  step-n must be positive\n");
                exit(1);
            }
            break;

        case 'v':
            verify = true;
            break;

        case '?':
            usage(argv[0]);
            exit(0);
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", ch);
        }
    }

    if (min_n > max_n) {
        fprintf(stderr, "ERROR:  max-n must be at least min-n\n");
        exit(1);
    }

    if (optind < argc) {
        fprintf(stderr, "ERROR:  unhandled arguments");
        while (optind < argc)
            fprintf(stderr, "  %s", argv[optind++]);
        fprintf(stderr, "\n");
        exit(1);
    }

    if (test_fn_name == NULL)
        test_fn_name = strdup("insert");

    fprintf(stderr, "%sList Performance Tester\n\n",
        list_type == ARRAY_LIST ? "Array-" : "Linked ");

    srandom(seed);

    // Allocate the appropriate kind of list.
    list_t *list = NULL;
    if (list_type == ARRAY_LIST) {
        list = alloc_arraylist();
    }
    else if (list_type == LINKED_LIST) {
        list = alloc_linkedlist();
    }
    else {
        assert(false); // Should never happen
    }

    run_perf_test(test_fn_name, test_fn, list, min_n, max_n, step_n, reps);
    free(test_fn_name);

    // Free the list.
    if (list_type == ARRAY_LIST) {
        free_arraylist(list);
    }
    else if (list_type == LINKED_LIST) {
        free_linkedlist(list);
    }
    else {
        assert(false); // Should never happen
    }

    return 0;
}
