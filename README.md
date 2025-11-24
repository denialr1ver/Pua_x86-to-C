# Pua_x86-to-C
LBYARCH MP2 (grayscale int to float)

**Group Members:**
* Pua, Daniel Joseph | S17B

## Performance Analysis
The function was tested on three image sizes (10x10, 100x100, 1000x1000) across 30 runs to get the average execution times for each. The Assembly code went through three iterations to optimize performance.

### Results (Time in microseconds)

| Version | 10x10 (100 px) | 100x100 (10k px) | 1000x1000 (1M px) | Result vs C |
| :--- | :--- | :--- | :--- | :--- |
| **C Language (Baseline)** | 0.21 µs | 20.16 µs | 2116.23 µs | - |
| **ASM Iteration 1 (Naive)** | 0.37 µs | 36.32 µs | 3840.90 µs | **Slower** |
| **ASM Iteration 2 (MULSS)** | 0.20 µs | 25.28 µs | 1942.42 µs | **Similar / Slightly Faster** |
| **ASM Iteration 3 (Unrolled)**| 0.07 µs | 5.07 µs | 533.97 µs | **~4x Faster** |

### Optimization Analysis

#### Iteration 1: Naive Approach (`DIVSS`)
* **Method:** Used the `DIVSS` instruction to divide each pixel by 255.0.
* **Result:** The Assembly code was significantly slower than the C code.
* **Reason:** Floating-point division is computationally expensive. The C compiler might have automatically optimized the division into a multiplication, which gives it an unfair advantage.

#### Iteration 2: Multiplication Trick (`MULSS`)
* **Method:** Replaced division with multiplication by the reciprocal using `MULSS`.
* **Result:** The Assembly code matched or slightly beat the C code.
* **Reason:** `MULSS` has much less latency than `DIVSS`. However, the CPU was still processing pixels one-by-one dependent on one register.

#### Iteration 3: Loop Unrolling (Final)
* **Method:** Processed 4 pixels per loop using four registers.
* **Result:** Achieved an approximate 4x speedup over the C version.
* **Reason:** This exploited Instruction Level Parallelism (I had to search this up). By breaking the dependency chain, the CPU could complete multiple multiplication instructions simultaneously while waiting for others to complete. It also reduced the number of loops necessary thereby reducing loop overhead.

## Correctness Check
The program automatically compares the output of the Assembly function against the C reference implementation. There is also an option to manually input the pixel values to check the correctness manually.

* **Status:** PASSED
* **Errors:** 0 mismatches found.

## Program Output Screenshot
*(Final Run)*
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/c17d219c-7344-4a40-87db-9eeb450ef41d" />


## How to Run
* Open `run.bat` - Script to compile and run the project.
