#include <stdio.h>
#include <math.h>
#include <complex.h>

#define N 8
#define PI 3.14159265358979323846

// Bit reversal function same as before
void bit_reversal(complex double x[]) {
    int i, j, k;
    complex double temp;
    for (i = 1, j = N / 2; i < N - 1; i++) {
        if (i < j) {
            temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
        k = N / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}

void fft_dif(complex double x[]) {
    int step, half, k, m;
    complex double w, wm, t, u;

    // No bit reversal at input for DIF FFT!

    // Loop from largest butterflies to smallest
    for (step = N; step >= 2; step >>= 1) {
        half = step / 2;
        wm = cexp(-I * 2.0 * PI / step);

        for (k = 0; k < N; k += step) {
            w = 1.0 + 0.0 * I;
            for (m = 0; m < half; m++) {
                u = x[k + m];
                t = x[k + m + half];

                // Butterfly operation (DIF):
                x[k + m] = u + t;
                x[k + m + half] = (u - t) * w;

                w *= wm;
            }
        }
    }

    // Bit reversal on output to get natural order
    bit_reversal(x);
}

int main() {
    complex double x[N];
    int i;

    // Sample input sequence (can change this)
    for (i = 0; i < N; i++) {
        x[i] = i + 0.0 * I;
        printf("x[%d] = %.4f + %.4fi\n", i, creal(x[i]), cimag(x[i]));
    }

    fft_dif(x);

    printf("\nDIF FFT result:\n");
    for (i = 0; i < N; i++) {
        printf("X[%d] = %.4f + %.4fi\n", i, creal(x[i]), cimag(x[i]));
    }

    return 0;
}
