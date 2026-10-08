#include "NeedlemanWunsch.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {

        cout << "Usage:\n";
        cout << "NeedlemanWunschSequential <sequenceA> <sequenceB>\n";

        return 1;
    }

    string fileA = argv[1];
    string fileB = argv[2];

    string sequenceA = readSequence(fileA);
    string sequenceB = readSequence(fileB);

    if (sequenceA.empty() || sequenceB.empty()) {

        cerr << "Error: Sequence is empty." << endl;
        return 1;
    }

    cout << "========================================\n";
    cout << "Sequential Needleman-Wunsch\n";
    cout << "========================================\n";

    cout << "Sequence A length: "
         << sequenceA.length() << endl;

    cout << "Sequence B length: "
         << sequenceB.length() << endl;

    cout << "Match: " << MATCH << endl;
    cout << "Mismatch: " << MISMATCH << endl;
    cout << "Gap: " << GAP << endl;

    auto start = chrono::high_resolution_clock::now();

    int score = sequentialScore(sequenceA, sequenceB);

    auto end = chrono::high_resolution_clock::now();

    double time =
        chrono::duration<double, milli>(end - start).count();

    cout << "\nAlignment Score: "
         << score << endl;

    cout << "Execution Time: "
         << time << " ms" << endl;

    return 0;
}