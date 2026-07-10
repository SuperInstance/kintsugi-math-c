#include "../src/kintsugi.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASSERT_FEQ(a, b) assert(fabs((a) - (b)) < 1e-6)

void test_golden_repair() {
    GoldenSeam s = golden_repair(5, 10, 0.8);
    assert(s.severity == 5);
    assert(s.position == 10);
    ASSERT_FEQ(s.repair_quality, 0.8);
    printf("  PASS: golden_repair\n");
}

void test_golden_repair_clamp() {
    GoldenSeam s = golden_repair(3, 0, 1.5);
    ASSERT_FEQ(s.repair_quality, 1.0);
    printf("  PASS: golden_repair_clamp\n");
}

void test_wabi_sabi() {
    double vals[] = {1.0, 2.0, 3.0, 4.0};
    double recovered[] = {1.0, 2.0, 3.0};
    WabiSabiReport r = wabi_sabi(vals, 4, recovered, 3);
    assert(r.golden_ratio > 0);
    assert(r.aesthetic_score > 0);
    assert(r.entropy >= 0);
    printf("  PASS: wabi_sabi\n");
}

void test_wabi_sabi_empty() {
    WabiSabiReport r = wabi_sabi(NULL, 0, NULL, 0);
    ASSERT_FEQ(r.entropy, 0.0);
    ASSERT_FEQ(r.golden_ratio, 0.0);
    printf("  PASS: wabi_sabi_empty\n");
}

void test_fragment_collection() {
    FragmentCollection fc = fragment_collection_create(10);
    assert(fc.count == 0);
    assert(fc.capacity == 10);
    
    double d1[] = {1.0, 2.0, 3.0};
    double d2[] = {4.0, 5.0, 6.0};
    
    fragment_collection_add(&fc, d1, 3, 1.0, 0);
    fragment_collection_add(&fc, d2, 3, 1.0, 3);
    assert(fc.count == 2);
    
    size_t out_len;
    double *result = fragment_reassemble(&fc, &out_len);
    assert(result != NULL);
    assert(out_len == 6);
    ASSERT_FEQ(result[0], 1.0);
    ASSERT_FEQ(result[3], 4.0);
    
    free(result);
    fragment_collection_destroy(&fc);
    printf("  PASS: fragment_collection\n");
}

void test_fragment_overlap() {
    FragmentCollection fc = fragment_collection_create(10);
    double d1[] = {1.0, 2.0};
    double d2[] = {10.0, 20.0};
    
    fragment_collection_add(&fc, d1, 2, 1.0, 0);
    fragment_collection_add(&fc, d2, 2, 1.0, 1); /* overlaps at position 1 */
    
    size_t out_len;
    double *result = fragment_reassemble(&fc, &out_len);
    assert(result != NULL);
    assert(out_len == 3);
    ASSERT_FEQ(result[0], 1.0);
    /* Position 1: average of 2.0 and 10.0 = 6.0 */
    ASSERT_FEQ(result[1], 6.0);
    
    free(result);
    fragment_collection_destroy(&fc);
    printf("  PASS: fragment_overlap\n");
}

void test_crack_graph() {
    CrackGraph cg = crack_graph_create(5);
    crack_graph_add(&cg, 0, 1, 0.8);
    crack_graph_add(&cg, 1, 2, 0.6);
    crack_graph_add(&cg, 2, 3, 0.4);
    assert(cg.crack_count == 3);
    
    size_t joint_count;
    GoldenJoint *joints = find_golden_joints(&cg, &joint_count);
    assert(joint_count > 0);
    
    double resilience = measure_resilience(&cg, joints, joint_count);
    assert(resilience > 0);
    
    free(joints);
    crack_graph_destroy(&cg);
    printf("  PASS: crack_graph\n");
}

void test_crack_graph_empty() {
    CrackGraph cg = crack_graph_create(3);
    double resilience = measure_resilience(&cg, NULL, 0);
    ASSERT_FEQ(resilience, 0.0);
    crack_graph_destroy(&cg);
    printf("  PASS: crack_graph_empty\n");
}

int main() {
    printf("Running kintsugi-math-c tests:\n");
    test_golden_repair();
    test_golden_repair_clamp();
    test_wabi_sabi();
    test_wabi_sabi_empty();
    test_fragment_collection();
    test_fragment_overlap();
    test_crack_graph();
    test_crack_graph_empty();
    printf("\n8 passed, 0 failed\n");
    return 0;
}
