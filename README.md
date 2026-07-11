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

Signatures below match `src/kintsugi.h` exactly. The collection/graph
`create` functions return the struct **by value** and take a capacity/count
argument (see Quick Start for usage).

### Golden Seam
- `GoldenSeam golden_repair(int severity, int position, double quality)` — Build a seam; `quality` is clamped to `[0, 1]`

### Fragment Collection
- `FragmentCollection fragment_collection_create(size_t capacity)` — Create an empty collection with the given fragment capacity
- `void fragment_collection_destroy(FragmentCollection *fc)` — Free the collection and its fragment data
- `int fragment_collection_add(FragmentCollection *fc, const double *data, size_t len, double confidence, size_t offset)` — Add a fragment; returns `0` on success, `-1` on full/alloc failure
- `double *fragment_reassemble(const FragmentCollection *fc, size_t *out_len)` — Reassemble into complete data (confidence-weighted); returns `NULL` on alloc failure. Caller must `free()` the result.

### Crack Graph
- `CrackGraph crack_graph_create(int node_count)` — Create an empty graph for `node_count` nodes
- `void crack_graph_destroy(CrackGraph *cg)` — Free the crack list
- `int crack_graph_add(CrackGraph *cg, int source, int target, double weight)` — Add a failure edge; returns `0` on success, `-1` on alloc failure
- `GoldenJoint *find_golden_joints(const CrackGraph *cg, size_t *joint_count)` — Rank repair points by impact/connectivity; out-of-range endpoints are ignored. Returns `NULL` for empty/invalid graphs. Caller must `free()`.
- `double measure_resilience(const CrackGraph *cg, const GoldenJoint *joints, size_t joint_count)` — Compute graph-aware resilience score (average node survival after fault damage and repairs); out-of-range endpoints are ignored

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
- `WabiSabiReport wabi_sabi(const double *values, size_t len, const double *recovered, size_t recovered_len)` — Compute the full report in one call. The returned struct exposes the three metrics below as fields (there are no separate free functions for them):
  - `.entropy` — `log(1 + sqrt(variance))` of `values` (`0` when `len == 0`)
  - `.golden_ratio` — `recovered_len / len` (`0` when `len == 0`)
  - `.aesthetic_score` — `golden_ratio / (1 + entropy)`


## How It Works

Same algorithms as the Python/npm versions, implemented in pure C with manual memory management. Fragment reassembly uses confidence-weighted interpolation. Crack graphs are adjacency lists. Resilience measures the average node survival after fault damage propagates through the graph and `GoldenJoint` repairs are applied: each crack contributes damage to its source and target nodes, base survival is derived from that damage, and a joint at a node increases that node's survival by its `beauty`, clamped to a maximum of 1.0.

## License

MIT
