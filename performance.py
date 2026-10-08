import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

PROJECT_DIR = Path(__file__).resolve().parent
RESULTS_DIR = PROJECT_DIR / "results"
GRAPHS_DIR = RESULTS_DIR / "graphs"
FILE = RESULTS_DIR / "performance.csv"

GRAPHS_DIR.mkdir(parents=True, exist_ok=True)

if not FILE.is_file():
    raise SystemExit(
        f"Missing {FILE}. Generate it by compiling and running "
        "src/PerformanceAnalysis.cpp with an OpenMP-enabled C++ toolchain."
    )

data = pd.read_csv(FILE)

# Remove spaces from column names
data.columns = data.columns.str.strip()

print("\n========== PERFORMANCE DATA ==========")
print(data.to_string(index=False))

print("\n========== COLUMNS ==========")
print(data.columns.tolist())

print("\n========== THREADS ==========")
print(data["threads"].unique())

print("======================================\n")


# ---------------------------------------------------------
# Graph 1: Sequence Size vs Execution Time
# ---------------------------------------------------------

parallel_8 = data[data["threads"] == 8]

# If 8 threads are not present, use the maximum thread count
if parallel_8.empty:
    max_threads = data["threads"].max()

    print(
        f"WARNING: 8 threads not found. "
        f"Using {max_threads} threads instead."
    )

    parallel_8 = data[data["threads"] == max_threads]

plt.figure(figsize=(10, 6))

plt.plot(
    parallel_8["size"],
    parallel_8["seq_ms"],
    marker="o",
    linewidth=2,
    label="Sequential"
)

plt.plot(
    parallel_8["size"],
    parallel_8["par_ms"],
    marker="o",
    linewidth=2,
    label=f"OpenMP - {parallel_8['threads'].iloc[0]} Threads"
)

plt.xlabel("Sequence Size")
plt.ylabel("Execution Time (ms)")
plt.title("Sequence Size vs Execution Time")

plt.legend()
plt.grid(True)

plt.tight_layout()

plt.savefig(
    GRAPHS_DIR / "execution_time.png",
    dpi=300,
    bbox_inches="tight"
)

plt.close()


# ---------------------------------------------------------
# Graph 2: Threads vs Speedup
# ---------------------------------------------------------

plt.figure(figsize=(10, 6))

for size in sorted(data["size"].unique()):

    subset = data[data["size"] == size].sort_values("threads")

    if subset.empty:
        continue

    plt.plot(
        subset["threads"],
        subset["speedup"],
        marker="o",
        linewidth=2,
        label=str(size)
    )

plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title("Threads vs Speedup")

plt.legend(title="Sequence Size")
plt.grid(True)

plt.tight_layout()

plt.savefig(
    GRAPHS_DIR / "speedup.png",
    dpi=300,
    bbox_inches="tight"
)

plt.close()


# ---------------------------------------------------------
# Graph 3: Threads vs Efficiency
# ---------------------------------------------------------

plt.figure(figsize=(10, 6))

for size in sorted(data["size"].unique()):

    subset = data[data["size"] == size].sort_values("threads")

    if subset.empty:
        continue

    plt.plot(
        subset["threads"],
        subset["efficiency"],
        marker="o",
        linewidth=2,
        label=str(size)
    )

plt.xlabel("Number of Threads")
plt.ylabel("Efficiency")
plt.title("Threads vs Efficiency")

plt.legend(title="Sequence Size")
plt.grid(True)

plt.tight_layout()

plt.savefig(
    GRAPHS_DIR / "efficiency.png",
    dpi=300,
    bbox_inches="tight"
)

plt.close()


print("========================================")
print("Graphs generated successfully!")
print("========================================")

print(GRAPHS_DIR / "execution_time.png")
print(GRAPHS_DIR / "speedup.png")
print(GRAPHS_DIR / "efficiency.png")