#include <stdio.h>
#include <math.h>
#include <complex.h>
#define N 8
#define PI 3.14159265358979323846
double magnitude[N];
double phase[N];
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
void fft(complex double x[]) {
    int step, half, k, m;
    complex double w, wm, t, u;

    bit_reversal(x);

    for (step = 2; step <= N; step <<= 1) {
        half = step / 2;
        wm = cexp(-I * 2.0 * PI / step);

        for (k = 0; k < N; k += step) {
            w = 1.0 + 0.0 * I;
            for (m = 0; m < half; m++) {
                t = w * x[k + m + half];
                u = x[k + m];
                x[k + m] = u + t;
                x[k + m + half] = u - t;
                w *= wm;
            }
        }
    }
}
int main() {
    complex double x[N];
    int i;
    printf("Input sequence:\n");
    for (i = 0; i < N; i++) {
        x[i] = i + 0.0 * I;
        printf("x[%d] = %.4f + %.4fi\n", i, creal(x[i]), cimag(x[i]));
    }
    fft(x);
    printf("\nDIT FFT result:\n");
    int k;
    for (k=0; k< N; k++){
        magnitude[k] = sqrt(creal(x[k]) * creal(x[k]) +cimag(x[k]) * cimag(x[k]));
        phase[k] = atan2(cimag(x[k]), creal(x[k]));
        printf("X[%d] = %f + j%f | Magnitude = %f | Phase = %f radians\n",
                 k, creal(x[k]), cimag(x[k]), magnitude[k], phase[k]);
    }

    return 0;
}
