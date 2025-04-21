#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <algorithm> // for std::swap
#include <iomanip>   // for std::setprecision

// Function to read data from file
std::vector<int> read_data(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Cannot open input file: " + filename);
    }

    int size;
    infile >> size;
    if (infile.fail() || size <= 0) {
         throw std::runtime_error("Invalid or non-positive size in file: " + filename);
    }

    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        if (!(infile >> data[i])) {
             throw std::runtime_error("Error reading data or not enough elements in file: " + filename);
        }
    }
    infile.close();
    return data;
}

// Function to write data and time to file
void write_results(const std::string& filename, const std::vector<int>& data, double time_taken) {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }

    outfile << data.size() << std::endl; // Write size
    for (size_t i = 0; i < data.size(); ++i) {
        outfile << data[i] << (i == data.size() - 1 ? "" : "\n"); // Write sorted data
    }
    outfile << std::endl; // Add a newline before time for clarity
    outfile << std::fixed << std::setprecision(6) << time_taken << std::endl; // Write time

    outfile.close();
}

// Sequential Odd-Even Sort implementation
void odd_even_sort_sequential(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return; // Already sorted or empty

    bool is_sorted = false;
    for (int phase = 0; phase < n && !is_sorted; ++phase) {
        is_sorted = true; // Assume sorted until a swap occurs

        // Odd phase
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                is_sorted = false;
            }
        }

        // Even phase
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                is_sorted = false;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << std::endl;
        std::cerr << "Example: " << argv[0] << " input_10k.txt output_seq_10k.txt" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    try {
        // Read data
        std::vector<int> data = read_data(input_filename);
        std::cout << "Read " << data.size() << " elements from " << input_filename << std::endl;

        // Time and execute sort
        auto start_time = std::chrono::high_resolution_clock::now();
        odd_even_sort_sequential(data);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        double time_taken = elapsed.count();

        std::cout << "Sequential Sort completed in " << std::fixed << std::setprecision(6) << time_taken << " seconds." << std::endl;

        // Write results
        write_results(output_filename, data, time_taken);
        std::cout << "Results written to " << output_filename << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}