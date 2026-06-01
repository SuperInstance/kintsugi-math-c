#ifndef KINTSUGI_MATH_H
#define KINTSUGI_MATH_H

#include <stddef.h>

/* A golden seam — where a crack was repaired */
typedef struct {
    int severity;       /* 0-10 severity of the original crack */
    int position;       /* Position in the data sequence */
    double repair_quality; /* 0-1 quality of the repair */
} GoldenSeam;

/* A fragment of broken data */
typedef struct {
    double *data;
    size_t len;
    double confidence;  /* 0-1 how confident we are in this fragment */
    size_t offset;      /* Original position */
} Fragment;

/* Fragment collection for reassembly */
typedef struct {
    Fragment *fragments;
    size_t count;
    size_t capacity;
} FragmentCollection;

/* Wabi-sabi imperfection metrics */
typedef struct {
    double entropy;         /* Shannon entropy of error distribution */
    double golden_ratio;    /* Recovered / (Recovered + Lost) */
    double aesthetic_score; /* 0-1 overall beauty score */
} WabiSabiReport;

/* Crack in the system — error propagation edge */
typedef struct {
    int source;     /* Component where crack starts */
    int target;     /* Component where crack propagates */
    double weight;  /* Propagation strength */
} Crack;

/* Crack graph — error propagation network */
typedef struct {
    Crack *cracks;
    size_t crack_count;
    int node_count;
} CrackGraph;

/* Golden joint — optimal repair point */
typedef struct {
    int node;           /* Component to repair */
    double impact;      /* How much repair helps */
    double difficulty;  /* How hard to repair */
    double beauty;      /* Aesthetic value of the repair */
} GoldenJoint;

/* Core functions */
GoldenSeam golden_repair(int severity, int position, double quality);
WabiSabiReport wabi_sabi(const double *values, size_t len, 
                         const double *recovered, size_t recovered_len);

/* Fragment operations */
FragmentCollection fragment_collection_create(size_t capacity);
void fragment_collection_destroy(FragmentCollection *fc);
int fragment_collection_add(FragmentCollection *fc, const double *data, 
                            size_t len, double confidence, size_t offset);
double *fragment_reassemble(const FragmentCollection *fc, size_t *out_len);

/* Crack graph operations */
CrackGraph crack_graph_create(int node_count);
void crack_graph_destroy(CrackGraph *cg);
int crack_graph_add(CrackGraph *cg, int source, int target, double weight);
GoldenJoint *find_golden_joints(const CrackGraph *cg, size_t *joint_count);

/* Resilience testing */
double measure_resilience(const CrackGraph *cg, const GoldenJoint *joints, 
                         size_t joint_count);
#endif
