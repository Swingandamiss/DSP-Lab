#include <stdio.h>
#include <math.h>
#define N 8
#define PI 3.141592653589793

// Generate sinusoidal samples
float x_real[N];  // Real part (samples of the sinusoid)
float x_imag[N] = {0.0};  // Imaginary part (assumed zero)
float X_real[N];
float X_imag[N];

int main()
{
    int k, n;
    float freq = 1.0;   // Frequency in cycles per N samples (can be changed)
    float amplitude = 1.0;
    float phase = 0.0;

    // Generate samples: x[n] = amplitude * sin(2*pi*freq*n/N + phase)
    for (n = 0; n < N; n++) {
        x_real[n] = amplitude * sin(2 * PI * freq * n / N + phase);
        // x_imag[n] = 0.0; already initialized above
    }

    // DFT calculation
    for (k = 0; k < N; k++) {
        X_real[k] = 0.0;
        X_imag[k] = 0.0;
        for (n = 0; n < N; n++) {
            float angle = -2 * PI * k * n / N;
            X_real[k] += x_real[n] * cos(angle) - x_imag[n] * sin(angle);
            X_imag[k] += x_real[n] * sin(angle) + x_imag[n] * cos(angle);
        }
    }

    // Print DFT output
    printf("DFT Output of Sinusoidal Signal:\n");
    for (k = 0; k < N; k++) {
        printf("X[%d] = %f + j%f\n", k, X_real[k], X_imag[k]);
    }
    return 0;
}