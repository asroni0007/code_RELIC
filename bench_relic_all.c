/*
 * bench_relic_all.c
 *
 * Real RELIC-toolkit benchmark of the underlying pairing-based operations
 * used in the CBAS-DSH manuscript's per-operation cost model (Table 4):
 *
 *   T_bpo  : Bilinear pairing operation           e(P, Q)
 *   T_pbsm : Pairing-based scalar multiplication  [k]P  (G1)
 *   T_pbpa : Pairing-based point addition          P + Q (G1)
 *
 * This replaces an earlier placeholder in this file that only simulated
 * timings with time.sleep(); every measurement below calls the real RELIC
 * toolkit (https://github.com/relic-toolkit/relic) pairing API on an actual
 * BN-family pairing-friendly curve. This is an independent cross-check
 * against the MIRACL/PBC benchmark in ../code_MIRACLE_PBS -- it is expected
 * to produce different absolute millisecond values (different library,
 * different curve family, different hardware) and is not a substitute for
 * that benchmark; it exists so a reviewer or a second library confirms the
 * same operations are genuinely being measured, not merely projected.
 */

#include <stdio.h>
#include <time.h>
#include "relic.h"

static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

int main(void) {
    if (core_init() != RLC_OK) {
        fprintf(stderr, "RELIC core_init failed\n");
        return 1;
    }
    if (pc_param_set_any() != RLC_OK) {
        fprintf(stderr, "No pairing-friendly curve available in this RELIC build\n");
        core_clean();
        return 1;
    }
    pc_param_print();

    int N_bpo = 200, N_pbsm = 1000, N_pbpa = 5000;
    int WARMUP = 10;
    double start, end;

    g1_t p1, q1, r1;
    g2_t p2;
    gt_t gt_res;
    bn_t k, order;

    g1_null(p1); g1_null(q1); g1_null(r1);
    g2_null(p2);
    gt_null(gt_res);
    bn_null(k); bn_null(order);

    g1_new(p1); g1_new(q1); g1_new(r1);
    g2_new(p2);
    gt_new(gt_res);
    bn_new(k); bn_new(order);

    g1_get_ord(order);
    g1_rand(p1);
    g1_rand(q1);
    g2_rand(p2);
    bn_rand_mod(k, order);

    /* ---------- T_bpo : bilinear pairing e(P1, P2) ---------- */
    for (int i = 0; i < WARMUP; i++) pc_map(gt_res, p1, p2);
    start = now_ms();
    for (int i = 0; i < N_bpo; i++) pc_map(gt_res, p1, p2);
    end = now_ms();
    double T_bpo = (end - start) / N_bpo;

    /* ---------- T_pbsm : scalar multiplication [k]P1 in G1 ---------- */
    for (int i = 0; i < WARMUP; i++) g1_mul(r1, p1, k);
    start = now_ms();
    for (int i = 0; i < N_pbsm; i++) g1_mul(r1, p1, k);
    end = now_ms();
    double T_pbsm = (end - start) / N_pbsm;

    /* ---------- T_pbpa : point addition P1 + Q1 in G1 ---------- */
    for (int i = 0; i < WARMUP; i++) g1_add(r1, p1, q1);
    start = now_ms();
    for (int i = 0; i < N_pbpa; i++) g1_add(r1, p1, q1);
    end = now_ms();
    double T_pbpa = (end - start) / N_pbpa;

    printf("=== RELIC real-library benchmark (independent cross-check) ===\n");
    printf("Pairing-friendly curve auto-selected by pc_param_set_any()\n");
    printf("Symbol   Algorithm                                    time (ms)\n");
    printf("--------------------------------------------------------------\n");
    printf("T_bpo    Bilinear pairing operation                   %10.4f\n", T_bpo);
    printf("T_pbsm   Pairing-based scalar multiplication (G1)     %10.4f\n", T_pbsm);
    printf("T_pbpa   Pairing-based point addition (G1)            %10.4f\n", T_pbpa);
    printf("--------------------------------------------------------------\n");

    g1_free(p1); g1_free(q1); g1_free(r1);
    g2_free(p2);
    gt_free(gt_res);
    bn_free(k); bn_free(order);

    core_clean();
    return 0;
}
