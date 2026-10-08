#include "NeedlemanWunsch.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {

        cout << "Usage:\n";
        cout << "NeedlemanWunschOpenMP "
             << "<sequenceA> <sequenceB> <threads>\n";

        return 1;
    }

    string fileA = argv[1];
    string fileB = argv[2];

    int threads = stoi(argv[3]);

    string sequenceA = readSequence(fileA);
    string sequenceB = readSequence(fileB);

    if (sequenceA.empty() || sequenceB.empty()) {

        cerr << "Error: Sequence is empty." << endl;
        return 1;
    }

    cout << "========================================\n";
    cout << "OpenMP Parallel Needleman-Wunsch\n";
    cout << "========================================\n";

    cout << "Sequence A length: "
         << sequenceA.length() << endl;

    cout << "Sequence B length: "
         << sequenceB.length() << endl;

    cout << "Threads: "
         << threads << endl;

    cout << "OpenMP available processors: "
         << omp_get_num_procs() << endl;

    auto start = chrono::high_resolution_clock::now();

    int score =
        parallelScore(
            sequenceA,
            sequenceB,
            threads
        );

    auto end = chrono::high_resolution_clock::now();

    double time =
        chrono::duration<double, milli>(
            end - start
        ).count();

    cout << "\nAlignment Score: "
         << score << endl;

    cout << "Execution Time: "
         << time << " ms" << endl;

    return 0;
}