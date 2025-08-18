#include <stdio.h>
#include <math.h>

#define N 8
#define PI 3.14

float X_real[N] = {0.0, -1.417957, 2.011154, 1.422057, 3.999896, 1.401042, 1.966601, -1.387733};
float X_imag[N] = {0.0, 3.409803, -1.99974, -0.582083, 0.922297, 0.591800, 1.999766, -3.444952};
float x_reconstructed[N];

int main() {
    int n, k;
    float angle, real_sum, imag_sum;

    for (n = 0; n < N; n++) {
        real_sum = 0.0;
        imag_sum = 0.0;
        for (k = 0; k < N; k++) {
            angle = 2 * PI * k * n / N;
            real_sum += X_real[k] * cos(angle) - X_imag[k] * sin(angle);
            imag_sum += X_real[k] * sin(angle) + X_imag[k] * cos(angle);
        }
        // We only take the real part since original x[n] is real
        x_reconstructed[n] = real_sum / N;
    }

    printf("Reconstructed x[n] using IDFT:\n");
    for (n = 0; n < N; n++) {
        printf("x[%d] = %f\n", n, x_reconstructed[n]);
    }

    return 0;
}
