#include "NeedlemanWunsch.h"
#include <filesystem>

using namespace std;

int main() {

    filesystem::create_directories("results");

    ofstream output(
        "results/correctness.csv"
    );

    output << "size,threads,seq_score,"
           << "parallel_score,result\n";

    int sizes[] = {
        100,
        500,
        1000,
        2000,
        5000,
        10000
    };

    int threads[] = {
        1,
        2,
        4,
        8
    };

    int sizeCount =
        sizeof(sizes) / sizeof(sizes[0]);

    int threadCount =
        sizeof(threads) / sizeof(threads[0]);

    cout << "========================================\n";
    cout << "Correctness Testing\n";
    cout << "========================================\n\n";

    bool allPassed = true;

    for (int s = 0; s < sizeCount; s++) {

        int size = sizes[s];

        string fileA =
            "datasets/data_" +
            to_string(size) +
            "_A.txt";

        string fileB =
            "datasets/data_" +
            to_string(size) +
            "_B.txt";

        string A = readSequence(fileA);
        string B = readSequence(fileB);

        if (A.empty() || B.empty()) {

            cerr << "Dataset missing: "
                 << size << endl;

            continue;
        }

        int sequential =
            sequentialScore(A, B);

        for (int t = 0;
             t < threadCount;
             t++) {

            int th = threads[t];

            int parallel =
                parallelScore(A, B, th);

            string result =
                (sequential == parallel)
                ? "PASS"
                : "FAIL";

            if (result == "FAIL")
                allPassed = false;

            cout << "Size = "
                 << size
                 << ", Threads = "
                 << th
                 << ", Sequential = "
                 << sequential
                 << ", Parallel = "
                 << parallel
                 << ", "
                 << result
                 << endl;

            output
                << size << ","
                << th << ","
                << sequential << ","
                << parallel << ","
                << result << "\n";
        }
    }

    output.close();

    cout << "\n========================================\n";

    if (allPassed)
        cout << "ALL TESTS PASSED\n";
    else
        cout << "SOME TESTS FAILED\n";

    cout << "========================================\n";

    return 0;
}