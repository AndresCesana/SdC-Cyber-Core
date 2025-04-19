#include <stdio.h>

// Function to process an array of floats
// - Converts the float array to an integer array by casting each element
// - Calculates and prints average of the integer array
void process_array(float *arr, int length) {
    // Check if the array is empty
    if (length == 0) {
        printf("Empty array received.\n");
        return;
    }

    // Declare an integer array to store the casted values
    int int_array[length];
    int sum = 0; // Variable to store the sum of the integer array

    // Print the converted integer array
    printf("Converted integer array: [");
    for (int i = 0; i < length; i++) {
        // Cast each float value to an integer and store it in the integer array
        int_array[i] = (int)arr[i];
        // Add the integer value to the sum
        sum += int_array[i];
        // Print the current integer value
        printf("%d", int_array[i]);
        // Add a comma separator for all but the last element
        if (i < length - 1)
            printf(", ");
    }
    printf("]\n");

    // Calculate the average of the integer array
    double average = (double)sum / length;
    // Print the calculated average
    printf("Average of casted integers: %.2f\n", average);
}
