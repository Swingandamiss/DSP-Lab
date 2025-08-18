#include <stdio.h>
#include <math.h>
#define N 8
#define PI 3.14
float x_real[N] = {1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0};
float x_imag[N] = {0.0};
float X_real[N];
float X_imag[N];
float magnitude[N];
float phase[N];
int main() {
    int k, n;
    float angle;

    for (k = 0; k < N; k++) {
        X_real[k] = 0.0;
        X_imag[k] = 0.0;
        for (n = 0; n < N; n++) {
            angle = -2 * PI * k * n / N;
            X_real[k] += x_real[n] * cos(angle) - x_imag[n] * sin(angle);
            X_imag[k] += x_real[n] * sin(angle) + x_imag[n] * cos(angle);
        }
    }
    printf("DFT Output:\n");
    for (k = 0; k < N; k++) {
        magnitude[k] = sqrt(X_real[k] * X_real[k] + X_imag[k] * X_imag[k]);
        phase[k] = atan2(X_imag[k], X_real[k]);
        printf("X[%d] = %f + j%f | Magnitude = %f | Phase = %f radians\n",
               k, X_real[k], X_imag[k], magnitude[k], phase[k]);
    }
    return 0;
}
