#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    string filename;
    cout << "Enter the name of the file: ";
    cin >> filename;

    char ch;

    
    auto start = high_resolution_clock::now();
    ifstream file1(filename, ios::in);
    if (file1.is_open()) {
        cout << "\nReading without Buffer:\n";
        while (file1.get(ch)) {
            cout << ch;
        }
        file1.close();
    }
    auto end = high_resolution_clock::now();
    cout << "\nReading without Buffer took: "
        << duration_cast<milliseconds>(end - start).count()
        << " ms\n";

   
    cout << "\n-----------------------------\n" << endl;

    
    start = high_resolution_clock::now();
    ifstream file2(filename, ios::in);
    if (file2.is_open()) {
        cout << "Reading with Buffer:\n";
        char buffer[4096]; 
        while (file2.read(buffer, sizeof(buffer))) {
            cout.write(buffer, file2.gcount());
        }
        file2.close();
    }
    end = high_resolution_clock::now();
    cout << "\nReading with Buffer took: "
        << duration_cast<milliseconds>(end - start).count()
        << " ms\n";

    return 0;
}
