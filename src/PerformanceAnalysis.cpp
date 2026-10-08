#include "NeedlemanWunsch.h"
#include <filesystem>
#include <iomanip>

using namespace std;

const int REPEATS = 5;

const int SIZES[] = {
    100,
    500,
    1000,
    2000,
    5000,
    10000
};

const int THREADS[] = {
    1,
    2,
    4,
    8
};


// ---------------------------------------------------------
// Calculate median
// ---------------------------------------------------------
double median(vector<double> values) {

    sort(values.begin(), values.end());

    int n = values.size();

    if (n % 2 == 1)
        return values[n / 2];

    return (
        values[n / 2 - 1] +
        values[n / 2]
    ) / 2.0;
}


// ---------------------------------------------------------
// Measure sequential execution time
// ---------------------------------------------------------
double measureSequential(
    const string& A,
    const string& B
) {

    vector<double> times;

    for (int r = 0;
         r < REPEATS;
         r++) {

        auto start =
            chrono::high_resolution_clock::now();

        volatile int score =
            sequentialScore(A, B);

        auto end =
            chrono::high_resolution_clock::now();

        double time =
            chrono::duration<double, milli>(
                end - start
            ).count();

        times.push_back(time);
    }

    return median(times);
}


// ---------------------------------------------------------
// Measure parallel execution time
// ---------------------------------------------------------
double measureParallel(
    const string& A,
    const string& B,
    int threads
) {

    vector<double> times;

    for (int r = 0;
         r < REPEATS;
         r++) {

        auto start =
            chrono::high_resolution_clock::now();

        volatile int score =
            parallelScore(
                A,
                B,
                threads
            );

        auto end =
            chrono::high_resolution_clock::now();

        double time =
            chrono::duration<double, milli>(
                end - start
            ).count();

        times.push_back(time);
    }

    return median(times);
}


// ---------------------------------------------------------
// Main experiment
// ---------------------------------------------------------
int main() {

    filesystem::create_directories("results");

    ofstream output(
        "results/performance.csv"
    );

    output << "size,threads,"
           << "seq_ms,par_ms,"
           << "speedup,efficiency,"
           << "seq_score,par_score,"
           << "match\n";

    cout << fixed << setprecision(3);

    cout << "========================================\n";
    cout << "Performance Analysis\n";
    cout << "========================================\n\n";

    int sizeCount =
        sizeof(SIZES) / sizeof(SIZES[0]);

    int threadCount =
        sizeof(THREADS) / sizeof(THREADS[0]);

    for (int s = 0;
         s < sizeCount;
         s++) {

        int size = SIZES[s];

        string fileA =
            "datasets/data_" +
            to_string(size) +
            "_A.txt";

        string fileB =
            "datasets/data_" +
            to_string(size) +
            "_B.txt";

        string A =
            readSequence(fileA);

        string B =
            readSequence(fileB);

        if (A.empty() || B.empty()) {

            cerr << "Dataset missing: "
                 << size << endl;

            continue;
        }

        cout << "\nSequence size: "
             << size
             << " x "
             << size
             << endl;

        int sequentialResult =
            sequentialScore(A, B);

        double sequentialTime =
            measureSequential(A, B);

        for (int t = 0;
             t < threadCount;
             t++) {

            int threads = THREADS[t];

            int parallelResult =
                parallelScore(
                    A,
                    B,
                    threads
                );

            double parallelTime =
                measureParallel(
                    A,
                    B,
                    threads
                );

            double speedup =
                sequentialTime /
                parallelTime;

            double efficiency =
                speedup /
                threads;

            string match =
                (sequentialResult ==
                 parallelResult)
                ? "PASS"
                : "FAIL";

            cout << "Threads: "
                 << threads
                 << " | Sequential: "
                 << sequentialTime
                 << " ms"
                 << " | Parallel: "
                 << parallelTime
                 << " ms"
                 << " | Speedup: "
                 << speedup
                 << " | Efficiency: "
                 << efficiency
                 << " | "
                 << match
                 << endl;

            output
                << size << ","
                << threads << ","
                << sequentialTime << ","
                << parallelTime << ","
                << speedup << ","
                << efficiency << ","
                << sequentialResult << ","
                << parallelResult << ","
                << match
                << "\n";
        }
    }

    output.close();

    cout << "\n========================================\n";
    cout << "Performance results saved to:\n";
    cout << "results/performance.csv\n";
    cout << "========================================\n";

    return 0;
}