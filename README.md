# code_RELIC — fix notes (read before committing)

## What was wrong

The previous `bench_relic_all.c` in this repository was not a real benchmark.
Despite the `.c` extension and filename, its content was a Python script that
used `time.sleep(random.uniform(...))` to fabricate plausible-looking timings
for `T_bpo`, `T_pbsm`, and `T_pbpa`. No RELIC library, or any cryptographic
library, was actually invoked. The file contained an explicit comment saying
this should be replaced with a real implementation, which was apparently
never done before the repository was shared with reviewers.

This is a reproducibility/integrity risk: a reviewer who clones this
repository expecting a RELIC-based benchmark would find a fake stub instead.
We found no evidence that this specific file's output was used to produce
any number reported in the manuscript (the real MIRACL/PBC benchmark in
`../code_MIRACLE_PBS` independently reproduces plausible values consistent
with the manuscript's Table 4), but the file needed to either be fixed or
removed before final submission.

## What this replacement does

`bench_relic_all.c` now genuinely links against the RELIC toolkit
(https://github.com/relic-toolkit/relic) and measures, on a real
pairing-friendly curve auto-selected by `pc_param_set_any()`:

- `T_bpo`  — a real bilinear pairing `pc_map(gt, P1, P2)`
- `T_pbsm` — a real scalar multiplication `g1_mul([k]P1)`
- `T_pbpa` — a real point addition `g1_add(P1, Q1)`

Each is warmed up 10 times, then timed over N iterations
(200 / 1000 / 5000 respectively) using `clock_gettime(CLOCK_MONOTONIC, ...)`,
matching the warm-up-then-time methodology already used in
`../code_MIRACLE_PBS/bench_miracl_all_3.cpp`.

This is offered as an **independent cross-check against a second pairing
library**, not a replacement for the MIRACL/PBC benchmark that actually
produced the manuscript's reported Table 4/5 numbers. Expect different
absolute millisecond values here (different library, different curve
family/parameters, different hardware) — what matters is that the operations
are now genuinely executed, not simulated.

## Build instructions

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

## Verified sample output

Produced on an independent Linux x86-64 sandbox (not the authors' own
hardware), included here for provenance. See `sample_output.txt`:

```
-- Curve BN-P256:
=== RELIC real-library benchmark (independent cross-check) ===
Pairing-friendly curve auto-selected by pc_param_set_any()
Symbol   Algorithm                                    time (ms)
--------------------------------------------------------------
T_bpo    Bilinear pairing operation                       1.1071
T_pbsm   Pairing-based scalar multiplication (G1)         0.1224
T_pbpa   Pairing-based point addition (G1)                0.0005
--------------------------------------------------------------
```

These values are plausible for a real pairing operation and are in the same
order of magnitude as the PBC-library and manuscript-reported figures,
supporting that the earlier dummy file's numbers (if ever consulted) were
not systematically misleading -- but they should not be copied into the
manuscript, since they come from an unrelated, uncontrolled machine.

## Action items for the corresponding author

1. Review and commit this file (or your own real RELIC implementation) to
   replace the placeholder.
2. Re-run on your own benchmark hardware if you want RELIC figures reported
   anywhere in the manuscript (currently none are; this is a cross-check
   utility only).
3. Consider adding a `.gitignore`/CI check that fails the build if any
   benchmark source file does not actually link against its claimed
   cryptographic library, to prevent this from happening again silently.
