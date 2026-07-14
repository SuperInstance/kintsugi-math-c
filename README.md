# kintsugi-math-c

> The mathematics of beautiful error recovery in C — golden seams, fragment reassembly, crack graphs, and wabi-sabi metrics.

## What This Does

`kintsugi-math-c` is the C implementation of kintsugi mathematics. It transforms errors into structured recovery objects, reassembles fragmented data with confidence-weighted interpolation, models crack propagation in dependency graphs, and measures resilience and aesthetic quality. Use it for fault-tolerant embedded systems, resilience testing in C/C++ projects, or building recovery-oriented architectures.

## The Cultural Root

See `kintsugi-math` (PyPI) for the full cultural background. Kintsugi (金継ぎ) teaches that breakage is part of history — repairs should be beautiful, not hidden.

## Install

```bash
# Clone and build
git clone https://github.com/SuperInstance/kintsugi-math-c.git
cd kintsugi-math-c
make
# Or integrate the single header/source into your project
```

## Quick Start

```c
#include <stdio.h>
#include <stdlib.h>
#include "kintsugi.h"

int main(void) {
    // Fragment reassembly
    FragmentCollection fc = fragment_collection_create(10);
    double f1[] = {1.0, 2.0};
    double f2[] = {3.0, 4.0};
    fragment_collection_add(&fc, f1, 2, 0.9, 0);
    fragment_collection_add(&fc, f2, 2, 0.7, 2);

    size_t out_len;
    double *result = fragment_reassemble(&fc, &out_len);
    // result = [1.0, 2.0, 3.0, 4.0]

    free(result);
    fragment_collection_destroy(&fc);

    // Crack propagation graph
    CrackGraph cg = crack_graph_create(5);
    crack_graph_add(&cg, /*source=*/0, /*target=*/1, /*weight=*/0.8);
    crack_graph_add(&cg, 1, 2, 0.6);

    GoldenJoint joints[] = {
        {.node = 1, .impact = 0.9, .difficulty = 0.5, .beauty = 0.9}
    };
    double resilience = measure_resilience(&cg, joints, 1);
    printf("Resilience: %.3f\n", resilience);

    crack_graph_destroy(&cg);
    return 0;
}
```

## API Reference

### Fragment Collection
- `FragmentCollection *fragment_collection_create()` — Create an empty collection
- `void fragment_collection_destroy(FragmentCollection *fc)` — Free memory
- `int fragment_collection_add(FragmentCollection *fc, const double *data, size_t data_len, double confidence, size_t offset)` — Add a fragment
- `double *fragment_reassemble(const FragmentCollection *fc, size_t *out_len)` — Reassemble into complete data

### Crack Graph
- `CrackGraph *crack_graph_create()` — Create empty graph
- `void crack_graph_destroy(CrackGraph *cg)`
- `int crack_graph_add(CrackGraph *cg, int source, int target, double weight)` — Add failure edge
- `double measure_resilience(const CrackGraph *cg, const GoldenJoint *joints, size_t n_joints)` — Compute graph-aware resilience score (average node survival after fault damage and repairs)

### Golden Joint
```c
typedef struct {
    int node;           /* Component to repair */
    double impact;      /* How much repair helps */
    double difficulty;  /* How hard to repair */
    double beauty;      /* Aesthetic value of the repair */
} GoldenJoint;
```

### Wabi-Sabi
- `double wabi_sabi_entropy(const double *values, size_t n)` — Shannon entropy
- `double golden_ratio(size_t recovered, size_t total)` — Golden ratio proximity
- `double aesthetic_score(const double *values, size_t n)` — 0–1 beauty score

## How It Works

Same algorithms as the Python/npm versions, implemented in pure C with manual memory management. Fragment reassembly uses confidence-weighted interpolation. Crack graphs are adjacency lists. Resilience measures the average node survival after fault damage propagates through the graph and `GoldenJoint` repairs are applied: each crack contributes damage to its source and target nodes, base survival is derived from that damage, and a joint at a node increases that node's survival by its `beauty`, clamped to a maximum of 1.0.

## License

MIT
