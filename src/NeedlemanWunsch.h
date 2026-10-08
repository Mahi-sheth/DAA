#ifndef NEEDLEMAN_WUNSCH_H
#define NEEDLEMAN_WUNSCH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;

const int MATCH = 1;
const int MISMATCH = -1;
const int GAP = -2;


// ---------------------------------------------------------
// Read DNA sequence from TXT or FASTA file
// ---------------------------------------------------------
string readSequence(const string& filename) {

    ifstream file(filename);

    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return "";
    }

    string line;
    string sequence;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        // Ignore FASTA header
        if (line[0] == '>')
            continue;

        for (char c : line) {

            c = toupper(c);

            if (c == 'A' || c == 'T' ||
                c == 'C' || c == 'G') {

                sequence += c;
            }
        }
    }

    file.close();

    return sequence;
}


// ---------------------------------------------------------
// Sequential Needleman-Wunsch
// Score only
// Uses two rows to reduce memory
// ---------------------------------------------------------
int sequentialScore(const string& a, const string& b) {

    int n = a.length();
    int m = b.length();

    vector<int> previous(m + 1);
    vector<int> current(m + 1);

    for (int j = 0; j <= m; j++) {
        previous[j] = j * GAP;
    }

    for (int i = 1; i <= n; i++) {

        current[0] = i * GAP;

        for (int j = 1; j <= m; j++) {

            int diagonal =
                previous[j - 1] +
                (a[i - 1] == b[j - 1] ? MATCH : MISMATCH);

            int up =
                previous[j] + GAP;

            int left =
                current[j - 1] + GAP;

            current[j] = max(diagonal, max(up, left));
        }

        swap(previous, current);
    }

    return previous[m];
}


// ---------------------------------------------------------
// Parallel Needleman-Wunsch using OpenMP
// Wavefront / Anti-Diagonal method
// ---------------------------------------------------------
int parallelScore(const string& a,
                  const string& b,
                  int threads) {

    int n = a.length();
    int m = b.length();

    if (n == 0)
        return m * GAP;

    if (m == 0)
        return n * GAP;

    if (threads < 1)
        threads = 1;

    /*
       Three buffers are used:

       previousPrevious -> diagonal d-2
       previous         -> diagonal d-1
       current          -> diagonal d
    */

    vector<vector<int>> buffers(
        3,
        vector<int>(n + 2, 0)
    );

    int* previousPrevious = buffers[0].data();
    int* previous = buffers[1].data();
    int* current = buffers[2].data();

    // d = 0
    previousPrevious[0] = 0;

    // d = 1
    previous[0] = GAP;
    previous[1] = GAP;

    int lastDiagonal = n + m;

    omp_set_dynamic(0);

    #pragma omp parallel num_threads(threads) \
                         shared(previousPrevious, previous, current)
    {

        for (int d = 2; d <= lastDiagonal; d++) {

            int low = max(1, d - m);
            int high = min(n, d - 1);

            // Initialize boundary cells
            #pragma omp single
            {
                if (d <= m)
                    current[0] = d * GAP;

                if (d <= n)
                    current[d] = d * GAP;
            }

            // Cells on the same anti-diagonal are independent
            #pragma omp for schedule(static)
            for (int i = low; i <= high; i++) {

                int j = d - i;

                int diagonal =
                    previousPrevious[i - 1] +
                    (a[i - 1] == b[j - 1]
                         ? MATCH
                         : MISMATCH);

                int up =
                    previous[i - 1] + GAP;

                int left =
                    previous[i] + GAP;

                current[i] =
                    max(diagonal, max(up, left));
            }

            // Wait until all cells of this diagonal are complete
            #pragma omp barrier

            // Rotate buffers
            #pragma omp single
            {
                int* temp = previousPrevious;
                previousPrevious = previous;
                previous = current;
                current = temp;
            }

            #pragma omp barrier
        }
    }

    return previous[n];
}

#endif