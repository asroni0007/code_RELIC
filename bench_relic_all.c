import time
import random

# --------------------------------------------------------------------
# 1. Fungsi Dummy Operasi Pairing
#    (Ganti bagian ini dengan implementasi pairing sebenarnya)
# --------------------------------------------------------------------

def dummy_bilinear_pairing_operation():
    """Simulasi operasi pairing e(P, Q) (T_bpo)."""
    time.sleep(random.uniform(0.001, 0.003))   # simulasi waktu
    return "e(P,Q)"

def dummy_scalar_mul():
    """Simulasi scalar multiplication P = a·G (T_pbsm)."""
    time.sleep(random.uniform(0.0005, 0.0015))
    return "a·G"

def dummy_point_add():
    """Simulasi point addition R = P + Q (T_pbpa)."""
    time.sleep(random.uniform(0.0004, 0.0010))
    return "P+Q"

# --------------------------------------------------------------------
# 2. Wrapper Pengukuran Waktu
# --------------------------------------------------------------------

def measure_time(func, repeat=10):
    """Mengukur rata-rata waktu eksekusi function."""
    total = 0
    for _ in range(repeat):
        start = time.time()
        func()
        end = time.time()
        total += (end - start)
    return (total / repeat) * 1000  # dalam milidetik

# --------------------------------------------------------------------
# 3. Hitung Semua Operasi Pairing
# --------------------------------------------------------------------

T_bpo  = measure_time(dummy_bilinear_pairing_operation)
T_pbsm = measure_time(dummy_scalar_mul)
T_pbpa = measure_time(dummy_point_add)

# --------------------------------------------------------------------
# 4. Cetak Hasil (Sesuai Arahan)
# --------------------------------------------------------------------

print("=== Summary (ms) ===")
print(f"T_bpo   : {T_bpo:.4f} ms   # Bilinear Pairing Operation")
print(f"T_pbsm  : {T_pbsm:.4f} ms   # Point-based Scalar Multiplication")
print(f"T_pbpa  : {T_pbpa:.4f} ms   # Point-based Point Addition")

# --------------------------------------------------------------------
# 5. Contoh Rumus Total Waktu (Jika diperlukan)
# --------------------------------------------------------------------

T_total_example = 1*T_bpo + 4*T_pbsm + 3*T_pbpa
print("\n=== Example Computation ===")
print(f"T_total_example = 1·T_bpo + 4·T_pbsm + 3·T_pbpa = {T_total_example:.4f} ms")