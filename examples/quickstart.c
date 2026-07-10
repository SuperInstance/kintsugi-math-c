#include <stdio.h>
#include <stdlib.h>
#include "../src/kintsugi.h"

int main(void) {
    /* Fragment reassembly */
    FragmentCollection fc = fragment_collection_create(10);
    double f1[] = {1.0, 2.0};
    double f2[] = {3.0, 4.0};

    fragment_collection_add(&fc, f1, 2, 0.9, 0);
    fragment_collection_add(&fc, f2, 2, 0.7, 2);

    size_t out_len;
    double *result = fragment_reassemble(&fc, &out_len);
    if (result) {
        printf("Reassembled length: %zu\n", out_len);
        free(result);
    }
    fragment_collection_destroy(&fc);

    /* Crack propagation graph */
    CrackGraph cg = crack_graph_create(5);
    crack_graph_add(&cg, 0, 1, 0.8);
    crack_graph_add(&cg, 1, 2, 0.6);

    GoldenJoint joints[] = {
        {.node = 1, .impact = 0.9, .difficulty = 0.5, .beauty = 0.9}
    };
    double resilience = measure_resilience(&cg, joints, 1);
    printf("Resilience: %.3f\n", resilience);

    crack_graph_destroy(&cg);
    return 0;
}
