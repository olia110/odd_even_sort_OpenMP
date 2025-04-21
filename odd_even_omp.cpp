#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <algorithm> // for std::swap
#include <iomanip>   // for std::setprecision
#include <omp.h>     // OpenMP Header

// Function to read data from file (same as sequential version)
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

// Function to write data, time, and thread count to file
void write_results(const std::string& filename, const std::vector<int>& data, double time_taken, int num_threads) {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }

    outfile << data.size() << std::endl; // Write size
    for (size_t i = 0; i < data.size(); ++i) {
        outfile << data[i] << (i == data.size() - 1 ? "" : "\n"); // Write sorted data
    }
    outfile << std::endl; // Add a newline before time/threads for clarity
    outfile << std::fixed << std::setprecision(6) << time_taken << std::endl; // Write time
    outfile << num_threads << std::endl; // Write number of threads used

    outfile.close();
}

// Parallel Odd-Even Sort implementation using OpenMP
void odd_even_sort_parallel(std::vector<int>& arr, int num_threads) {
    int n = arr.size();
    if (n <= 1) return; // Already sorted or empty

    // Set the number of threads for OpenMP regions
    if (num_threads > 0) {
        omp_set_num_threads(num_threads);
    } else {
         // Use default or environment variable defined thread count
         #pragma omp parallel
         {
              #pragma omp master
              num_threads = omp_get_num_threads(); // Get the actual number used
         }
         std::cout << "Using default OpenMP threads: " << num_threads << std::endl;
    }


    bool is_sorted_global = false; // Overall sorted flag
    for (int phase = 0; phase < n && !is_sorted_global; ++phase) {
        // Use reduction for is_sorted. Initialize local is_sorted to true.
        // If *any* thread performs a swap, the final is_sorted_phase will be false.
        bool is_sorted_phase = true;

        // Odd phase - parallel
        #pragma omp parallel for shared(arr, n) reduction(&&:is_sorted_phase) schedule(static)
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                is_sorted_phase = false; // This thread made a swap
            }
        }
        // Implicit barrier here ensures all odd phase swaps are done

        // Even phase - parallel
        #pragma omp parallel for shared(arr, n) reduction(&&:is_sorted_phase) schedule(static)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                is_sorted_phase = false; // This thread made a swap
            }
        }
        // Implicit barrier here ensures all even phase swaps are done

        is_sorted_global = is_sorted_phase; // Update global flag after both parallel phases
    }
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename> <num_threads>" << std::endl;
        std::cerr << "Example: " << argv[0] << " input_10k.txt output_omp_10k_4t.txt 4" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    int num_threads = 0;

    try {
        num_threads = std::stoi(argv[3]);
         if (num_threads <= 0) {
             std::cout << "Warning: Non-positive thread count specified. Using OpenMP default." << std::endl;
             // Let OpenMP decide or use environment variable OMP_NUM_THREADS
             // We'll determine the actual count inside the sort function if needed
         }

        // Read data
        std::vector<int> data = read_data(input_filename);
        std::cout << "Read " << data.size() << " elements from " << input_filename << std::endl;

        // Time and execute sort
        auto start_time = std::chrono::high_resolution_clock::now();
        odd_even_sort_parallel(data, num_threads); // Pass thread count
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        double time_taken = elapsed.count();

        // Get the actual number of threads used if it was defaulted
        if (num_threads <= 0) {
            #pragma omp parallel
            {
                #pragma omp master
                 num_threads = omp_get_num_threads();
            }
        }


        std::cout << "Parallel Sort (" << num_threads << " threads) completed in "
                  << std::fixed << std::setprecision(6) << time_taken << " seconds." << std::endl;

        // Write results
        write_results(output_filename, data, time_taken, num_threads);
        std::cout << "Results written to " << output_filename << std::endl;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid number format for threads - " << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Thread count out of range - " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}