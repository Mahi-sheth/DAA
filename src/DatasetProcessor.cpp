#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Dataset sizes required for the project
const int SIZES[] = {
    100,
    500,
    1000,
    2000,
    5000,
    10000
};


// ---------------------------------------------------------
// Read DNA sequence from FASTA file
// ---------------------------------------------------------
string readFASTA(const string& filename)
{
    ifstream file(filename);

    if (!file)
    {
        cerr << "ERROR: Cannot open file: "
             << filename << endl;

        return "";
    }

    string line;
    string dna = "";

    while (getline(file, line))
    {
        // Skip empty lines
        if (line.empty())
            continue;

        // Skip FASTA header
        if (line[0] == '>')
            continue;

        // Keep only valid DNA characters
        for (int i = 0; i < (int)line.length(); i++)
        {
            char c = line[i];

            // Convert lowercase to uppercase
            if (c >= 'a' && c <= 'z')
                c = c - 'a' + 'A';

            if (c == 'A' ||
                c == 'T' ||
                c == 'C' ||
                c == 'G')
            {
                dna += c;
            }
        }
    }

    file.close();

    return dna;
}


// ---------------------------------------------------------
// Save DNA sequence to file
// ---------------------------------------------------------
bool saveSequence(
    const string& filename,
    const string& sequence)
{
    ofstream file(filename);

    if (!file)
    {
        cerr << "ERROR: Cannot create file: "
             << filename << endl;

        return false;
    }

    file << sequence;

    file.close();

    return true;
}


// ---------------------------------------------------------
// Main
// ---------------------------------------------------------
int main()
{
    cout << "========================================" << endl;
    cout << "     REAL DNA DATASET PROCESSOR" << endl;
    cout << "========================================" << endl;

    // Input FASTA file
    string fastaFile = "ecoli.fasta";

    cout << "\nReading: "
         << fastaFile << endl;

    string dna = readFASTA(fastaFile);

    // Check whether reading was successful
    if (dna.empty())
    {
        cerr << "\nERROR: DNA sequence is empty." << endl;
        cerr << "Make sure ecoli.fasta is in the project folder."
             << endl;

        return 1;
    }

    cout << "Total DNA length: "
         << dna.length()
         << endl;


    // Number of dataset sizes
    int numberOfSizes =
        sizeof(SIZES) / sizeof(SIZES[0]);


    cout << "\nCreating datasets...\n" << endl;


    // -----------------------------------------------------
    // Generate datasets
    // -----------------------------------------------------
    for (int k = 0; k < numberOfSizes; k++)
    {
        int size = SIZES[k];


        // We need enough DNA for both sequences
        // Sequence A starts at 0
        // Sequence B starts at 500
        if (500 + size > (int)dna.length())
        {
            cerr << "ERROR: Not enough DNA for size "
                 << size << endl;

            continue;
        }


        // Sequence A
        string A =
            dna.substr(0, size);


        // Sequence B
        string B =
            dna.substr(500, size);


        // File names
        string fileA =
            "datasets/data_" +
            to_string(size) +
            "_A.txt";

        string fileB =
            "datasets/data_" +
            to_string(size) +
            "_B.txt";


        // Save files
        bool savedA =
            saveSequence(fileA, A);

        bool savedB =
            saveSequence(fileB, B);


        if (savedA && savedB)
        {
            cout << "Created dataset: "
                 << size
                 << " x "
                 << size
                 << endl;

            cout << "   A -> "
                 << fileA
                 << endl;

            cout << "   B -> "
                 << fileB
                 << endl;
        }
        else
        {
            cout << "Failed to create dataset: "
                 << size
                 << endl;
        }
    }


    cout << "\n========================================" << endl;
    cout << "Dataset generation completed!" << endl;
    cout << "========================================" << endl;


    return 0;
}