# Biological Sequence Alignment using the Needleman-Wunsch Algorithm

## Project title
Biological Sequence Alignment using the Needleman-Wunsch Algorithm with OpenMP Parallelization

## Problem statement
Sequence alignment is a core problem in bioinformatics and computational biology. When comparing biological sequences such as DNA, RNA, or proteins, researchers need to determine the degree of similarity or divergence between them. The Needleman-Wunsch algorithm computes the optimal global alignment between two sequences using dynamic programming. In this project, real bacterial DNA fragments from E. coli K-12 MG1655 are aligned using a simple scoring scheme, and the implementation is parallelized with OpenMP.

## Objective
The project aims to:
- implement the Needleman-Wunsch algorithm in C++,
- compare the sequential and parallel versions,
- evaluate correctness across multiple dataset sizes,
- measure execution time, speedup, and efficiency for 1, 2, 4, and 8 threads,
- generate performance graphs from experimental results,
- document the algorithm and the observed behavior clearly for a DAA project submission.

## Background / Needleman-Wunsch algorithm
The Needleman-Wunsch algorithm is a classic dynamic-programming method for global sequence alignment. It creates a matrix where each cell represents the best alignment score between prefixes of the two sequences. The score at each cell is computed from the diagonal, upper, and left neighbors using the recurrence:

score(i, j) = max(
  score(i-1, j-1) + match_or_mismatch,
  score(i-1, j) + gap,
  score(i, j-1) + gap
)

This procedure yields the optimal global alignment score for the full sequences. The implementation in this repository uses the standard scoring model and a memory-efficient rolling-row design for the sequential version.

## Scoring system
The project follows the required scoring rules:
- Match = +1
- Mismatch = -1
- Gap = -2

These values are defined in `src/NeedlemanWunsch.h` as `MATCH`, `MISMATCH`, and `GAP`.

## Sequential approach
The sequential implementation is in `src/NeedlemanWunschSequential.cpp` and uses the shared helper functions in `src/NeedlemanWunsch.h`.

The algorithm:
1. Reads the DNA sequences from text files.
2. Initializes the dynamic-programming table.
3. Fills the table from top-left to bottom-right.
4. Uses a rolling two-row optimization to reduce memory usage.
5. Returns the final alignment score.

This approach has time complexity O(mn) and memory complexity O(m) for the row-based implementation when using the compact rolling-row form.

## OpenMP wavefront / anti-diagonal parallel approach
The parallel implementation is in `src/NeedlemanWunschOpenMP.cpp` and is based on the OpenMP wavefront or anti-diagonal strategy.

The implementation computes cells that belong to the same anti-diagonal (`i + j = constant`) in parallel, because those cells are independent with respect to the same wavefront step. The relevant logic appears in `parallelScore()` inside `src/NeedlemanWunsch.h` and uses three buffers: `previousPrevious`, `previous`, and `current`.

## Why anti-diagonal cells can be processed in parallel
Each cell in the DP matrix depends only on its left, upper, and diagonal neighbors. In a wavefront formulation, each anti-diagonal is processed as one set of cells. All cells on the same anti-diagonal can be computed independently once the previous anti-diagonal values are available. This is the key parallelization idea behind the OpenMP implementation: the dependency graph is diagonal-shaped, so one wavefront can advance while threads compute distinct cells within that wave.

## Dataset source and preparation
The project uses real DNA data from the E. coli K-12 MG1655 reference genome, NCBI accession NC_000913.3.

The source genome is stored as `ecoli.fasta` in the project root. The dataset generator reads the FASTA file, keeps only valid DNA bases (`A`, `C`, `G`, `T`), and creates paired sequence files in `datasets/`.

The generator logic is implemented in `src/DatasetProcessor.cpp`, and it creates the following dataset sizes:
- 100
- 500
- 1000
- 2000
- 5000
- 10000

The actual dataset files in the repository are:
- `datasets/data_100_A.txt`, `datasets/data_100_B.txt`
- `datasets/data_500_A.txt`, `datasets/data_500_B.txt`
- `datasets/data_1000_A.txt`, `datasets/data_1000_B.txt`
- `datasets/data_2000_A.txt`, `datasets/data_2000_B.txt`
- `datasets/data_5000_A.txt`, `datasets/data_5000_B.txt`
- `datasets/data_10000_A.txt`, `datasets/data_10000_B.txt`

## Team member roles
- Pranjali: Sequential Needleman-Wunsch implementation
- Fathima: OpenMP parallel wavefront implementation
- Maitreyi: Performance analysis, timing, speedup, efficiency and graphs
- Mahi: Dataset generation, correctness testing, documentation and project organization

## Project folder structure
```text
DAA2/
├── README.md
├── ecoli.fasta
├── performance.py
├── LLM_Usage_Log.md
├── docs/
│   └── report.md
├── src/
│   ├── NeedlemanWunsch.h
│   ├── NeedlemanWunschSequential.cpp
│   ├── NeedlemanWunschOpenMP.cpp
│   ├── DatasetProcessor.cpp
│   ├── CorrectnessTester.cpp
│   └── PerformanceAnalysis.cpp
├── datasets/
│   ├── data_100_A.txt
│   ├── data_100_B.txt
│   ├── data_500_A.txt
│   ├── data_500_B.txt
│   ├── data_1000_A.txt
│   ├── data_1000_B.txt
│   ├── data_2000_A.txt
│   ├── data_2000_B.txt
│   ├── data_5000_A.txt
│   ├── data_5000_B.txt
│   ├── data_10000_A.txt
│   └── data_10000_B.txt
├── results/
│   ├── correctness.csv
│   ├── performance.csv
│   └── graphs/
│       ├── execution_time.png
│       ├── speedup.png
│       └── efficiency.png
├── .gitignore
└── .venv/
```

## Compilation requirements
This project requires a C++17-compatible compiler with OpenMP support.

Important note: the source files use `#include <filesystem>` in `src/CorrectnessTester.cpp` and `src/PerformanceAnalysis.cpp`. This means a modern MinGW/MSYS2 toolchain is required; older GCC versions such as GCC 6.3.0 do not support the standard library filesystem API correctly. The local environment currently exposes `g++.exe` from MinGW 6.3.0, which fails with `filesystem: No such file or directory`, so the project should be built using a modern MinGW64/MSYS2 environment (for example GCC 10+ or newer).

## Exact Windows / MinGW / MSYS2 compilation commands
Run the following from a MinGW64 or MSYS2 UCRT64/CLANG64 terminal in the project root:

```bash
mkdir -p bin

g++ -std=c++17 -O2 -o bin/DatasetProcessor.exe src/DatasetProcessor.cpp

g++ -std=c++17 -O2 -fopenmp -o bin/NeedlemanWunschSequential.exe src/NeedlemanWunschSequential.cpp

g++ -std=c++17 -O2 -fopenmp -o bin/NeedlemanWunschOpenMP.exe src/NeedlemanWunschOpenMP.cpp

g++ -std=c++17 -O2 -fopenmp -o bin/CorrectnessTester.exe src/CorrectnessTester.cpp

g++ -std=c++17 -O2 -fopenmp -o bin/PerformanceAnalysis.exe src/PerformanceAnalysis.cpp
```

If you are using classic Windows CMD instead of MSYS2 shell, the same commands can be written with backslashes and `.exe` output names:

```cmd
mkdir bin

g++ -std=c++17 -O2 -o bin\DatasetProcessor.exe src\DatasetProcessor.cpp

g++ -std=c++17 -O2 -fopenmp -o bin\NeedlemanWunschSequential.exe src\NeedlemanWunschSequential.cpp

g++ -std=c++17 -O2 -fopenmp -o bin\NeedlemanWunschOpenMP.exe src\NeedlemanWunschOpenMP.cpp

g++ -std=c++17 -O2 -fopenmp -o bin\CorrectnessTester.exe src\CorrectnessTester.cpp

g++ -std=c++17 -O2 -fopenmp -o bin\PerformanceAnalysis.exe src\PerformanceAnalysis.cpp
```

## How to generate datasets
From the project root, run:

```bash
./bin/DatasetProcessor.exe
```

This reads `ecoli.fasta` and generates the required sequence pairs in `datasets/`.

## How to run the sequential implementation
```bash
./bin/NeedlemanWunschSequential.exe datasets/data_100_A.txt datasets/data_100_B.txt
```

This prints the alignment score and execution time for the chosen sequences.

## How to run the OpenMP implementation with 1, 2, 4 and 8 threads
```bash
./bin/NeedlemanWunschOpenMP.exe datasets/data_1000_A.txt datasets/data_1000_B.txt 1
./bin/NeedlemanWunschOpenMP.exe datasets/data_1000_A.txt datasets/data_1000_B.txt 2
./bin/NeedlemanWunschOpenMP.exe datasets/data_1000_A.txt datasets/data_1000_B.txt 4
./bin/NeedlemanWunschOpenMP.exe datasets/data_1000_A.txt datasets/data_1000_B.txt 8
```

The same pattern applies to all other dataset sizes.

## How to run correctness testing
```bash
./bin/CorrectnessTester.exe
```

This evaluates the sequential and OpenMP implementations across the required sizes and thread counts and writes the results to `results/correctness.csv`.

## How to run performance analysis
```bash
./bin/PerformanceAnalysis.exe
```

This measures median execution times over repeated runs, computes speedup and efficiency, and writes the final values to `results/performance.csv`.

## How to generate graphs
The project includes a Python script for plotting:

```bash
python performance.py
```

This reads `results/performance.csv` and creates:
- `results/graphs/execution_time.png`
- `results/graphs/speedup.png`
- `results/graphs/efficiency.png`

## Speedup and efficiency
Speedup is calculated as:

speedup = sequential_time / parallel_time

efficiency is calculated as:

efficiency = speedup / number_of_threads

These metrics are included directly in `results/performance.csv`.

The experimental data shows that the current implementation does not achieve strong speedup for the tested dataset sizes in this environment; for many cases the parallel version is slower than the sequential version, which indicates that synchronization and thread-creation overhead dominate the computation.

## Results
The results are stored in `results/performance.csv` and were generated by the actual performance analysis. The repository does not contain a `results/performance_summary.csv` file at the time of inspection.

The measured values show the following behavior for the Test dataset sizes from the CSV:

| Size | Sequential time (ms) | Parallel time at 8 threads (ms) | Speedup at 8 threads | Efficiency at 8 threads |
|------|----------------------|-------------------------------|---------------------|------------------------|
| 100 | 0.0227 | 153.7210 | 0.000147671 | 0.0000184588 |
| 500 | 0.6987 | 685.3410 | 0.00101949 | 0.000127437 |
| 1000 | 1.4506 | 1351.9600 | 0.00107296 | 0.00013412 |
| 2000 | 6.9011 | 2271.2700 | 0.00303843 | 0.000379804 |
| 5000 | 40.3007 | 5756.7700 | 0.00700057 | 0.000875071 |
| 10000 | 156.0960 | 11582.1000 | 0.0134773 | 0.00168467 |

These values are taken directly from `results/performance.csv` and indicate that, for this implementation and hardware configuration, parallel overhead is larger than the parallel benefit over the tested sequence sizes.

## Graphs
The repository contains the following generated plots in `results/graphs/`:

- [results/graphs/execution_time.png](results/graphs/execution_time.png)
- [results/graphs/speedup.png](results/graphs/speedup.png)
- [results/graphs/efficiency.png](results/graphs/efficiency.png)

These plots are generated by `python performance.py` and reflect the measurements stored in `results/performance.csv`.

## Correctness verification
The correctness results are stored in `results/correctness.csv`.

The file contains 24 rows, corresponding to the six dataset sizes and four thread counts (1, 2, 4, 8). Every row in the file has `PASS` in the `result` column, which means the parallel implementation matched the sequential score for every tested configuration.

Examples from the file:
- `100,1,-31,-31,PASS`
- `100,2,-31,-31,PASS`
- `5000,8,2500,2500,PASS`
- `10000,8,7500,7500,PASS`

This confirms that the OpenMP wavefront implementation is numerically consistent with the sequential implementation for the provided test data.

## Complexity analysis
Sequential version:
- Time complexity: O(mn)
- Space complexity: O(m) using rolling rows

Parallel OpenMP wavefront version:
- Work: O(mn)
- Parallelism is limited by anti-diagonal dependency structure
- Synchronization overhead from barriers and thread scheduling can dominate for small matrices

Thus, the theoretical parallelism is significant, but real runtime performance depends heavily on matrix size and platform overhead.

## Limitations
- The implementation focuses on correctness and a simple wavefront strategy rather than an optimized high-performance parallel alignment algorithm.
- For small sequences, the overhead of OpenMP parallelization exceeds the computation cost.
- The measured speedup values in `results/performance.csv` remain below 1 for most tested thread counts.
- The project does not compute an actual alignment path or backtrace, only the alignment score.

## Conclusion
This project demonstrates the implementation of a global sequence alignment algorithm in C++ using the Needleman-Wunsch method, with a parallel wavefront design using OpenMP. The correctness results confirm that the parallel algorithm matches the sequential algorithm for all evaluated dataset sizes and thread counts. The measured performance data shows that the current parallel approach introduces more overhead than benefit for the tested small-to-medium sequence sizes in this environment, but the project successfully demonstrates the core algorithmic and parallelization concepts involved in biological sequence alignment.

## Repository note
The project currently contains the actual generated CSV results and plots in `results/` and the dataset files in `datasets/`. No Java source files or `.class` files were found in the repository during inspection.
