#include "kintsugi.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Golden seam creation */
GoldenSeam golden_repair(int severity, int position, double quality) {
    GoldenSeam seam;
    seam.severity = severity;
    seam.position = position;
    seam.repair_quality = quality > 1.0 ? 1.0 : (quality < 0.0 ? 0.0 : quality);
    return seam;
}

/* Wabi-sabi metrics */
WabiSabiReport wabi_sabi(const double *values, size_t len,
                         const double *recovered, size_t recovered_len) {
    WabiSabiReport report = {0, 0, 0};
    
    /* Entropy of error distribution */
    if (len > 0) {
        double mean = 0;
        for (size_t i = 0; i < len; i++) mean += values[i];
        mean /= (double)len;
        
        double variance = 0;
        for (size_t i = 0; i < len; i++) {
            double diff = values[i] - mean;
            variance += diff * diff;
        }
        variance /= (double)len;
        report.entropy = log(1.0 + sqrt(variance));
    }
    
    /* Golden ratio of recovery */
    size_t total = recovered_len + (len - recovered_len);
    if (total > 0) {
        report.golden_ratio = (double)recovered_len / (double)total;
    }
    
    /* Aesthetic score: high recovery + low entropy = beautiful */
    report.aesthetic_score = report.golden_ratio * (1.0 / (1.0 + report.entropy));
    
    return report;
}

/* Fragment collection */
FragmentCollection fragment_collection_create(size_t capacity) {
    FragmentCollection fc;
    fc.fragments = (Fragment *)calloc(capacity, sizeof(Fragment));
    fc.count = 0;
    fc.capacity = capacity;
    return fc;
}

void fragment_collection_destroy(FragmentCollection *fc) {
    if (fc->fragments) {
        for (size_t i = 0; i < fc->count; i++) {
            free(fc->fragments[i].data);
        }
        free(fc->fragments);
        fc->fragments = NULL;
        fc->count = 0;
    }
}

int fragment_collection_add(FragmentCollection *fc, const double *data,
                            size_t len, double confidence, size_t offset) {
    if (fc->count >= fc->capacity) return -1;
    
    Fragment *f = &fc->fragments[fc->count];
    f->data = (double *)malloc(len * sizeof(double));
    if (!f->data) return -1;
    memcpy(f->data, data, len * sizeof(double));
    f->len = len;
    f->confidence = confidence;
    f->offset = offset;
    fc->count++;
    return 0;
}

double *fragment_reassemble(const FragmentCollection *fc, size_t *out_len) {
    /* Find total length needed */
    size_t max_offset = 0;
    size_t max_end = 0;
    for (size_t i = 0; i < fc->count; i++) {
        size_t end = fc->fragments[i].offset + fc->fragments[i].len;
        if (end > max_end) max_end = end;
        if (fc->fragments[i].offset > max_offset) max_offset = fc->fragments[i].offset;
    }
    
    *out_len = max_end;
    double *result = (double *)calloc(max_end, sizeof(double));
    if (!result) return NULL;
    
    /* Fill with fragments, weighted by confidence */
    double *weights = (double *)calloc(max_end, sizeof(double));
    if (!weights) { free(result); return NULL; }
    
    for (size_t i = 0; i < fc->count; i++) {
        const Fragment *f = &fc->fragments[i];
        for (size_t j = 0; j < f->len; j++) {
            size_t pos = f->offset + j;
            if (pos < max_end) {
                result[pos] += f->data[j] * f->confidence;
                weights[pos] += f->confidence;
            }
        }
    }
    
    /* Normalize */
    for (size_t i = 0; i < max_end; i++) {
        if (weights[i] > 0) result[i] /= weights[i];
    }
    
    free(weights);
    return result;
}

/* Crack graph */
CrackGraph crack_graph_create(int node_count) {
    CrackGraph cg;
    cg.cracks = NULL;
    cg.crack_count = 0;
    cg.node_count = node_count;
    return cg;
}

void crack_graph_destroy(CrackGraph *cg) {
    free(cg->cracks);
    cg->cracks = NULL;
    cg->crack_count = 0;
}

int crack_graph_add(CrackGraph *cg, int source, int target, double weight) {
    cg->cracks = (Crack *)realloc(cg->cracks, (cg->crack_count + 1) * sizeof(Crack));
    if (!cg->cracks) return -1;
    cg->cracks[cg->crack_count].source = source;
    cg->cracks[cg->crack_count].target = target;
    cg->cracks[cg->crack_count].weight = weight;
    cg->crack_count++;
    return 0;
}

GoldenJoint *find_golden_joints(const CrackGraph *cg, size_t *joint_count) {
    /* Count connections per node */
    double *impact = (double *)calloc(cg->node_count, sizeof(double));
    int *connections = (int *)calloc(cg->node_count, sizeof(int));
    
    for (size_t i = 0; i < cg->crack_count; i++) {
        impact[cg->cracks[i].source] += cg->cracks[i].weight;
        impact[cg->cracks[i].target] += cg->cracks[i].weight;
        connections[cg->cracks[i].source]++;
        connections[cg->cracks[i].target]++;
    }
    
    /* Find top joints (high impact, high connectivity) */
    *joint_count = 0;
    GoldenJoint *joints = (GoldenJoint *)malloc(cg->node_count * sizeof(GoldenJoint));
    
    for (int n = 0; n < cg->node_count; n++) {
        if (connections[n] > 0) {
            joints[*joint_count].node = n;
            joints[*joint_count].impact = impact[n];
            joints[*joint_count].difficulty = 1.0 / (1.0 + connections[n]);
            joints[*joint_count].beauty = impact[n] * (1.0 / (1.0 + connections[n]));
            (*joint_count)++;
        }
    }
    
    free(impact);
    free(connections);
    return joints;
}

double measure_resilience(const CrackGraph *cg, const GoldenJoint *joints,
                         size_t joint_count) {
    if (!cg || cg->node_count <= 0 || joint_count == 0) return 0.0;

    int n = cg->node_count;
    double *damage = (double *)calloc(n, sizeof(double));
    double *survival = (double *)malloc(n * sizeof(double));
    if (!damage || !survival) {
        free(damage);
        free(survival);
        return 0.0;
    }

    /* Accumulate crack damage at every node the crack touches. */
    double max_damage = 0.0;
    for (size_t i = 0; i < cg->crack_count; i++) {
        int s = cg->cracks[i].source;
        int t = cg->cracks[i].target;
        double w = cg->cracks[i].weight;

        if (s >= 0 && s < n) {
            damage[s] += w;
            if (damage[s] > max_damage) max_damage = damage[s];
        }
        if (t >= 0 && t < n) {
            damage[t] += w;
            if (damage[t] > max_damage) max_damage = damage[t];
        }
    }

    /* Base survival: less damage means higher survival. */
    for (int i = 0; i < n; i++) {
        if (max_damage > 0.0) {
            survival[i] = 1.0 - (damage[i] / max_damage);
            if (survival[i] < 0.0) survival[i] = 0.0;
        } else {
            survival[i] = 1.0;
        }
    }

    /* Apply golden-joint repairs at the nodes they cover. */
    for (size_t i = 0; i < joint_count; i++) {
        int node = joints[i].node;
        if (node < 0 || node >= n) continue;

        survival[node] += joints[i].beauty;
        if (survival[node] > 1.0) survival[node] = 1.0;
    }

    double total_survival = 0.0;
    for (int i = 0; i < n; i++) {
        total_survival += survival[i];
    }

    free(damage);
    free(survival);
    return total_survival / (double)n;
}
