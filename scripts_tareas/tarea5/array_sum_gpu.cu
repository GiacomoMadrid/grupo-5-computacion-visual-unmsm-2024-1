%%cuda
//Colab link:https://colab.research.google.com/drive/17VkDTCBgf8RmIdYF-2xNw7xzXiI44rEp?usp=sharing
#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <ctime>
#define size 1000000
using namespace std;

//DEVICE
__global__ void kernelVector_x_constant(float* arrayA, float* arrayB, float* sum_arr, int n) {
    // Get the index of the thread
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // If the thread index is valid
    if (idx < n) {
        // Add the elements from both arrays
        sum_arr[idx] = arrayA[idx] + arrayB[idx];
    }
}

// Function to check for CUDA errors
inline cudaError_t checkCuda(cudaError_t result) {
    if (result != cudaSuccess) {
        cerr << "CUDA Runtime Error: " << cudaGetErrorString(result) << endl;
        exit(-1);
    }
    return result;
}


//HOST
int main() {
    // Allocate memory on the host
    float* arrayA = new float[size];
    float* arrayB = new float[size];
    float* sum_arr = new float[size];
    float* arrayA_DEVICE = NULL;
    float* arrayB_DEVICE = NULL;
    float* sum_arr_DEVICE = NULL;

    // Initialize the arrays on the host
    for (int index = 0; index < size; index++) {
        arrayA[index] = index;
        arrayB[index] = index;
    }

    // Measure host sum time using ctime
    clock_t start_host = clock();
    // Sum on host
    for (int index = 0; index < size; index++) {
        sum_arr[index] = arrayA[index] + arrayB[index];
    }
    clock_t end_host = clock();
    double duration_host = double(end_host - start_host) / CLOCKS_PER_SEC;

    // Print the host first 100 results
    cout << "Host results: " << endl;
    for (int index = 0; index < 100; index++) {
        cout << sum_arr[index] << endl;
    }

    // Allocate memory on the device (same amount of bytes as on the host)
    checkCuda(cudaMalloc((void**)&arrayA_DEVICE, size * sizeof(float)));
    checkCuda(cudaMalloc((void**)&arrayB_DEVICE, size * sizeof(float)));
    checkCuda(cudaMalloc((void**)&sum_arr_DEVICE, size * sizeof(float)));

    // Copy the memory block from the host to the device
    checkCuda(cudaMemcpy(arrayA_DEVICE, arrayA, size * sizeof(float), cudaMemcpyHostToDevice));
    checkCuda(cudaMemcpy(arrayB_DEVICE, arrayB, size * sizeof(float), cudaMemcpyHostToDevice));

    // Initilize cudaEvent_t variables to measure device sum tume
    cudaEvent_t start_device, stop_device;
    checkCuda(cudaEventCreate(&start_device));
    checkCuda(cudaEventCreate(&stop_device));

    checkCuda(cudaEventRecord(start_device));
    // Execute CUDA kernel with 512 threads and 1000000/12 blocks
    kernelVector_x_constant<<<ceil(size/512), 512>>>(arrayA_DEVICE, arrayB_DEVICE, sum_arr_DEVICE, size);
    checkCuda(cudaEventRecord(stop_device));
    checkCuda(cudaEventSynchronize(stop_device));
    
    float duration_device = 0;
    checkCuda(cudaEventElapsedTime(&duration_device, start_device, stop_device));

    // Copy the processed data back to the host
    checkCuda(cudaMemcpy(sum_arr, sum_arr_DEVICE, size * sizeof(float), cudaMemcpyDeviceToHost));

    // Print the device first 100 results
    cout << "Device results: " << endl;
    for (int index = 0; index < 100; index++) {
        cout << sum_arr[index] << endl;
    }
    // Print execution times
    cout << "Host computation time: " << duration_host << " seconds" << endl;
    cout << "Device computation time: " << duration_device / 1000.0 << " seconds" << endl;

    // Free memory on the device
    cudaFree(sum_arr_DEVICE);
    cudaFree(arrayA_DEVICE);
    cudaFree(arrayB_DEVICE);
    // Free memory on the host
    delete[] arrayA;
    delete[] arrayB;
    delete[] sum_arr;
    // Destroy CUDA events
    cudaEventDestroy(start_device);
    cudaEventDestroy(stop_device);

    return 0;
}
