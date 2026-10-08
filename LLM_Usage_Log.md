# LLM Usage Log

This project used AI assistance during the development and documentation process. The following items reflect the work that is supported by the repository contents and the current implementation:

- Conversion of an earlier Java-based project plan and implementation concept into the final C++ project structure and source files.
- Conversion of the parallel algorithm from an initial design into the OpenMP wavefront / anti-diagonal formulation used in `src/NeedlemanWunsch.h` and `src/NeedlemanWunschOpenMP.cpp`.
- Assistance with generating the real DNA dataset workflow from `ecoli.fasta`, including sequence extraction and file creation logic in `src/DatasetProcessor.cpp`.
- Assistance with designing the correctness testing pipeline and validation output in `src/CorrectnessTester.cpp` and `results/correctness.csv`.
- Assistance with the performance analysis workflow in `src/PerformanceAnalysis.cpp` and the generation of `results/performance.csv`.
- Assistance with plotting the performance graphs using `performance.py` and producing the PNG outputs under `results/graphs/`.
- Debugging and build support for the C++/OpenMP toolchain, including compiler flags and environment-specific issues relevant to Windows/MSYS2/MinGW setups.
- Documentation help for the final project README and the formal report in `docs/report.md`.

This log is intentionally factual and limited to activities consistent with the code and artifacts present in the repository.
