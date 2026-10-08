# DAA Project Report: Needleman-Wunsch Sequence Alignment with OpenMP

## 1. Introduction
This project implements the Needleman-Wunsch algorithm for global biological sequence alignment using C++ and OpenMP. The objective is to compare a sequential dynamic-programming solution with a parallel wavefront/anti-diagonal approach and to study the effect of thread counts on execution time, speedup, and efficiency.

The project uses real DNA sequences from the E. coli K-12 MG1655 reference genome (NCBI accession NC_000913.3). The dataset is processed into multiple sequence sizes, and each sequence pair is evaluated using a standard scoring scheme:

- Match = +1
- Mismatch = -1
- Gap = -2

## 2. Problem Definition
Given two DNA sequences, we need to determine the optimal global alignment score between them. The Needleman-Wunsch algorithm is a dynamic-programming method that evaluates all possible alignments and selects the highest-scoring alignment under the defined penalty system.

## 3. Algorithm Overview
The algorithm constructs a 2D dynamic-programming table where each cell represents the best alignment score between prefixes of the two sequences. For a cell at position `(i, j)`, the score depends on:

- the diagonal neighbor `score(i-1, j-1)`,
- the upper neighbor `score(i-1, j)`,
- the left neighbor `score(i, j-1)`.

The recurrence is:

`score(i, j) = max(diagonal + match_or_mismatch, up + gap, left + gap)`

This produces the final alignment score at the bottom-right cell of the matrix.

## 4. Sequential Implementation
The sequential implementation is implemented in `src/NeedlemanWunschSequential.cpp` and uses helper functions defined in `src/NeedlemanWunsch.h`.

The sequential version uses a classic dynamic-programming table with a memory optimization that retains only the previous and current rows during computation. This keeps the implementation simple while preserving the correct global alignment logic.

### Sequential time complexity
- Time: O(mn)
- Space: O(m) for the rolling-row optimization

## 5. Parallel OpenMP Wavefront Implementation
The OpenMP implementation is implemented in `src/NeedlemanWunschOpenMP.cpp` and uses a wavefront or anti-diagonal parallelization strategy.

Each anti-diagonal of the DP matrix is independent once the previous anti-diagonals are available. This allows one diagonal to be processed in parallel by multiple threads, with each thread computing a subset of cells that belong to the same wavefront. The implementation uses a three-buffer scheme in the shared header file and OpenMP barriers to synchronize the diagonal updates.

### Why anti-diagonal parallelism works
Cells on the same anti-diagonal share the same `i + j` value. Their dependencies are limited to the neighboring diagonals, not to other cells on the same diagonal. Therefore, once the preceding diagonal is complete, all cells on the current diagonal can be processed concurrently.

## 6. Dataset Generation and Source Data
The raw biological input is stored in `ecoli.fasta`, which contains the E. coli K-12 MG1655 genome sequence. The file is read by `src/DatasetProcessor.cpp`, which selects valid nucleotide bases (`A`, `C`, `G`, `T`) and creates synthetic paired sequence files of fixed sizes.

The project uses these dataset sizes:

- 100
- 500
- 1000
- 2000
- 5000
- 10000

The generated files are saved in the `datasets/` folder as:

- `data_100_A.txt` / `data_100_B.txt`
- `data_500_A.txt` / `data_500_B.txt`
- `data_1000_A.txt` / `data_1000_B.txt`
- `data_2000_A.txt` / `data_2000_B.txt`
- `data_5000_A.txt` / `data_5000_B.txt`
- `data_10000_A.txt` / `data_10000_B.txt`

The actual inspected dataset lengths confirmed that each paired file contains exactly the specified number of characters.

## 7. Correctness Testing
The correctness verification is performed by `src/CorrectnessTester.cpp`.

It compares the sequential score and the parallel score for each dataset size and thread count across:

- 1 thread
- 2 threads
- 4 threads
- 8 threads

The output is written to `results/correctness.csv`.

### Correctness results from the repository
The file `results/correctness.csv` contains 24 rows for six sizes and four thread counts. Every row reports `PASS` in the `result` column, meaning the parallel implementation produced the same score as the sequential implementation for all tested cases.

Examples from the actual file:

- `100,1,-31,-31,PASS`
- `100,2,-31,-31,PASS`
- `5000,8,2500,2500,PASS`
- `10000,8,7500,7500,PASS`

This confirms correctness of the OpenMP wavefront design for the provided datasets.

## 8. Performance Analysis
The performance benchmark is implemented in `src/PerformanceAnalysis.cpp`.

For each dataset size, it measures the sequential and parallel execution times across 1, 2, 4, and 8 threads. The program computes:

- `seq_ms` (median sequential time)
- `par_ms` (median parallel time)
- `speedup = seq_ms / par_ms`
- `efficiency = speedup / threads`

The final results are written to `results/performance.csv`.

## 9. Results from Actual CSV Data
The actual repository data used for this report is in `results/performance.csv`.

The observed results are:

| Size | Sequential (ms) | Parallel (8 threads, ms) | Speedup (8 threads) | Efficiency (8 threads) |
|------|-----------------|--------------------------|---------------------|------------------------|
| 100 | 0.0227 | 153.7210 | 0.000147671 | 0.0000184588 |
| 500 | 0.6987 | 685.3410 | 0.00101949 | 0.000127437 |
| 1000 | 1.4506 | 1351.9600 | 0.00107296 | 0.00013412 |
| 2000 | 6.9011 | 2271.2700 | 0.00303843 | 0.000379804 |
| 5000 | 40.3007 | 5756.7700 | 0.00700057 | 0.000875071 |
| 10000 | 156.0960 | 11582.1000 | 0.0134773 | 0.00168467 |

These values are directly taken from `results/performance.csv` and demonstrate that the current OpenMP implementation does not produce speedup for the tested dataset sizes under the current environment and synchronization overhead.

## 10. Graphs
The performance plots were generated using the Python script `performance.py` and saved in `results/graphs/`.

The actual graph files present in the repository are:

- `results/graphs/execution_time.png`
- `results/graphs/speedup.png`
- `results/graphs/efficiency.png`

These graphs visualize the execution time trends, thread-scaling speedup, and efficiency values recorded in the performance CSV.

## 11. Complexity and Practical Interpretation
The theoretical complexity of the Needleman-Wunsch algorithm is O(mn), where m and n are the sequence lengths. The sequential implementation reduces space usage by only keeping previous rows. The OpenMP wavefront method preserves the same mathematical work, but introduces synchronization cost due to barriers between anti-diagonal layers.

In practice, for small matrices the overhead of launching and synchronizing threads outweighs the computational benefit. This is visible in the measured performance data, where speedup remains far below 1 for most thread counts.

## 12. Limitations
The project has several limitations:

- It computes only the alignment score, not the actual aligned sequences.
- It uses a simple wavefront OpenMP strategy rather than a highly optimized parallel alignment design.
- For small sequence sizes, the thread synchronization overhead dominates the runtime.
- The implementation measures performance on a limited number of fixed DNA sizes and thread counts.

## 13. Conclusion
This project successfully implements a global sequence alignment method using the Needleman-Wunsch algorithm in C++. The sequential and OpenMP implementations are functionally equivalent according to the correctness CSV, and the repository includes generated experimental data and graph outputs for analysis.

Although the project demonstrates the parallel wavefront concept correctly, the measured performance indicates that the current OpenMP approach does not scale well on the tested dataset sizes in this environment. This outcome is valuable because it highlights the practical impact of synchronization overhead, dependencies, and workload size on parallel performance.

## 14. Repository Summary
The final project contains:

- real E. coli DNA data in `ecoli.fasta`,
- generated sequence datasets in `datasets/`,
- sequential and OpenMP implementations in `src/`,
- correctness and performance output in `results/`,
- graph generation via `performance.py`,
- documentation in `README.md` and `docs/report.md`.

No Java implementation files or `.class` files were found in the repository during inspection.
