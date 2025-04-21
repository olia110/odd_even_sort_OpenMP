#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <stdexcept>
#include <limits> // Required for numeric_limits

// Function to generate random data and write to a file
void generate_data(const std::string& filename, int size, int min_val = 0, int max_val = 10000) {
    if (size <= 0) {
        throw std::invalid_argument("Size must be positive.");
    }

    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }

    // --- Random Number Generation ---
    std::random_device rd; // Obtain a random number from hardware entropy source
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distrib(min_val, max_val); // Define the range

    // --- Write Data ---
    outfile << size << std::endl; // Write size first

    for (int i = 0; i < size; ++i) {
        outfile << distrib(gen) << (i == size - 1 ? "" : "\n"); // Write numbers, newline separated
    }
    // Add a final newline for consistency, some read loops might expect it
    outfile << std::endl;

    outfile.close();
    std::cout << "Successfully generated " << size << " random integers to " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <output_filename> <size> [min_value] [max_value]" << std::endl;
        std::cerr << "Example: " << argv[0] << " data_10000.txt 10000" << std::endl;
        std::cerr << "Example: " << argv[0] << " data_large.txt 50000 0 1000000" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int size = 0;
    int min_val = 0;
    int max_val = 10000; // Default range

    try {
        size = std::stoi(argv[2]);
        if (argc >= 4) {
            min_val = std::stoi(argv[3]);
        }
        if (argc >= 5) {
            max_val = std::stoi(argv[4]);
        }
        if (min_val > max_val) {
             throw std::invalid_argument("min_value cannot be greater than max_value.");
        }

        generate_data(filename, size, min_val, max_val);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid number format provided - " << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Number out of range - " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}