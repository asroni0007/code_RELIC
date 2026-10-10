# code_RELIC

Independent cross-check of the pairing-related unit costs used in the CBAS-DSH manuscript (JICT submission), using the RELIC toolkit (https://github.com/relic-toolkit/relic).

## Scope

`bench_relic_all.c` times three operations on a pairing-friendly curve selected by `pc_param_set_any()`:

| Symbol | Operation | RELIC call |
|---|---|---|
| `T_bpo` | Bilinear pairing | `pc_map(gt, P1, P2)` |
| `T_pbsm` | Scalar multiplication in G1 | `g1_mul(R, P1, k)` |
| `T_pbpa` | Point addition in G1 | `g1_add(R, P1, Q1)` |

Each operation is warmed up 10 times and then timed over N iterations (200, 1000 and 5000 respectively) with `clock_gettime(CLOCK_MONOTONIC)`. This is the same warm-up-then-time method used in `code_MIRACLE_PBS`.

This repository is a cross-check against a second pairing library. It is **not** the source of Table 4 or Table 5 of the manuscript. Those unit costs come from `code_MIRACLE_PBS` (MIRACL Core and PBC). Absolute values here differ because the library, curve family and hardware differ.

## Build

```bash
git clone https://github.com/relic-toolkit/relic.git
cd relic && mkdir build && cd build
cmake -DARITH=gmp -DFP_PRIME=256 \
      -DWITH="BN;DV;FP;FPX;EP;EPX;PP;PC;MD;CP" \
      -DCHECK=off -DTESTS=0 -DBENCH=0 -DSHLIB=off \
      -DTIMER=CYCLE -DWSIZE=64 -DSEED=UDEV ..
make -j4
```

Then, from this directory:

```bash
gcc -O2 -I<relic>/include -I<relic>/build/include \
    -o bench_relic_run bench_relic_all.c \
    <relic>/build/lib/librelic_s.a -lgmp -lm
./bench_relic_run
```

## Sample output

`sample_output.txt` holds one run (curve BN-P256) on a Linux x86-64 machine that is not the authors' benchmark hardware. It is illustrative only. Expect different numbers on your machine, and do not compare them directly with the MIRACL/PBC figures in the manuscript.
