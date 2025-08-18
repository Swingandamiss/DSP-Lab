#include <stdio.h>
#include <math.h>

#define N 8
#define PI 3.14

float x_real[N];
float x_imag[N] = {0.0};
float X_real[N];
float X_imag[N];

float magnitude[N];
float Phase[N];

int main(){
    int k, n;
    float freq = 1.0;
    float amplitude = 1.0;
    float phase = 0.0;
    for (n = 0; n < N; n++) {
        x_real[n] = amplitude * sin(2 * PI * freq * n / N + phase);
    }
    for (k = 0; k < N; k++) {
        X_real[k] = 0.0;
        X_imag[k] = 0.0;
        for (n = 0; n < N; n++) {
            float angle = -2 * PI * k * n / N;
            X_real[k] += x_real[n] * cos(angle) - x_imag[n] * sin(angle);
            X_imag[k] += x_real[n] * sin(angle) + x_imag[n] * cos(angle);
        }
    }
    printf("DFT Output of Sinusoidal Signal:\n");
    for (k = 0; k < N; k++) {
        magnitude[k] = sqrt(X_real[k] * X_real[k] + X_imag[k] * X_imag[k]);
        Phase[k] = atan2(X_imag[k], X_real[k]);
        printf("X[%d] = %f + j%f | Magnitude = %f | Phase = %f radians\n",
               k, X_real[k], X_imag[k], magnitude[k], Phase[k]);
    }
    return 0;
}
