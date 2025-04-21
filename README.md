# Parallel Odd-Even Sort using OpenMP

This repository contains C++ implementations of the Odd-Even Sort algorithm, developed as part of a lab assignment for the "Distributed and Parallel Programming" course at Taras Shevchenko National University of Kyiv, Faculty of Computer Science and Cybernetics.

The project includes:
1.  A sequential version of Odd-Even Sort.
2.  A parallel version implemented using OpenMP.
3.  A data generation utility.
4.  (Optional) A Python script to plot performance results.

The primary goal is to compare the execution time and calculate the speedup of the parallel version against the sequential version for different input sizes and thread counts.

## Requirements

*   **C++ Compiler:** A modern C++ compiler supporting C++11 (e.g., GCC, Clang).
*   **OpenMP:** The compiler environment must support OpenMP.
    *   **Linux:** Typically available with GCC (`sudo apt install build-essential`).
    *   **macOS:** Requires installation via Homebrew (see Compilation notes below). Use either `brew install llvm` or `brew install gcc`.
*   **(Optional) Python 3:** Required only for running the `plot_results.py` script.
    *   Python libraries: `matplotlib`, `numpy` (`pip install matplotlib numpy`).
*   **(Optional) Homebrew:** For macOS users to install a compiler with OpenMP support.

## File Structure
.
├── generate_data.cpp # Source code for data generator
├── odd_even_seq.cpp # Source code for sequential sort
├── odd_even_omp.cpp # Source code for parallel sort (OpenMP)
├── plot_results.py # (Optional) Python script to generate plots
├── odd_even_sort_performance.png # (Optional) Example output graph from plotter
├── README.md # This file
└── (Optional) lab_report.pdf # The final lab report document

## Compilation

Open your terminal in the project directory.

1.  **Data Generator:**
    ```bash
    g++ generate_data.cpp -o generate_data -std=c++11 -O2
    ```

2.  **Sequential Sort:**
    ```bash
    g++ odd_even_seq.cpp -o odd_even_seq -std=c++11 -O2
    ```

3.  **Parallel Sort (OpenMP):**
    *   **Linux (using GCC):**
        ```bash
        g++ odd_even_omp.cpp -o odd_even_omp -std=c++11 -fopenmp -O2
        ```
    *   **macOS (using GCC installed via Homebrew):**
        *(Replace `g++-13` with the actual version you installed, e.g., `g++-12`, `g++-14`)*
        ```bash
        g++-13 odd_even_omp.cpp -o odd_even_omp -std=c++11 -fopenmp -O2
        ```
    *   **macOS (using Clang installed via Homebrew LLVM):**
        *(Replace `/opt/homebrew/opt/llvm` with the output of `brew --prefix llvm`)*
        ```bash
        /opt/homebrew/opt/llvm/bin/clang++ odd_even_omp.cpp -o odd_even_omp -std=c++11 -fopenmp -O2 -L$(brew --prefix llvm)/lib -I$(brew --prefix llvm)/include
        ```

*Note: Using optimization flags like `-O2` or `-O3` is recommended for performance measurements.*

## Usage

1.  **Compile the programs** using the appropriate commands above.

2.  **Generate test data:**
    ```bash
    # Example: Generate a file with 100,000 integers
    ./generate_data input_100k.txt 100000
    ```

3.  **Run the sequential version:**
    ```bash
    # Example: Run sequential sort on 100k data
    ./odd_even_seq input_100k.txt output_seq_100k.txt
    ```
    *Execution time will be printed to the console and saved in the output file.*

4.  **Run the parallel version:**
    ```bash
    # Example: Run parallel sort on 100k data using 4 threads
    ./odd_even_omp input_100k.txt output_omp_100k_4t.txt 4

    # Example: Run parallel sort on 50k data using 8 threads
    ./odd_even_omp input_50k.txt output_omp_50k_8t.txt 8
    ```
    *Execution time and thread count will be printed and saved.*

5.  **(Optional) Plot results:**
    *   Make sure you have collected the timing data (manually or from output files).
    *   Update the data arrays inside `plot_results.py` with your measured times.
    *   Run the script:
        ```bash
        python plot_results.py
        ```
    *   This will display the graph and save it as `odd_even_sort_performance.png`.

## Results

The performance comparison showed that:
*   Parallelization overhead made the OpenMP version significantly slower for small input sizes (N=10,000).
*   For medium size (N=50,000), only 2 threads provided minimal speedup; 4 and 8 threads were slower than sequential.
*   For the largest size tested (N=100,000), speedup was achieved, but peaked at 2 threads (approx 1.69x) and decreased with more threads (4 threads: 1.52x, 8 threads: 1.35x), indicating poor scaling for this implementation/algorithm on the test system.

Refer to `lab_report.pdf` (if included) for the full analysis, data table, graph, and detailed conclusions.
