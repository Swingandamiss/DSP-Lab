#include <stdio.h>
#include <math.h>
#define PI 3.141592653589793
void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
void bit_reverse(double real[], double imag[], int N) {
    int i, j, k;
    j = 0;
    for (i = 0; i < N; i++) {
        if (i < j) {
            swap(&real[i], &real[j]);
            swap(&imag[i], &imag[j]);
        }
        k = N / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}
void FFT_DIF(double real[], double imag[], int N) {
    for (int step = N; step >= 2; step >>= 1) {
        int half = step / 2;
        double theta = -2 * PI / step;
        double w_real = cos(theta);
        double w_imag = sin(theta);
        for (int i = 0; i < N; i += step) {
            double u_real = 1.0, u_imag = 0.0;
            for (int j = 0; j < half; j++) {
                int i1 = i + j;
                int i2 = i + j + half;
                double t_real = real[i1] - real[i2];
                double t_imag = imag[i1] - imag[i2];
                real[i1] += real[i2];
                imag[i1] += imag[i2];
                real[i2] = t_real * u_real - t_imag * u_imag;
                imag[i2] = t_real * u_imag + t_imag * u_real;
                // update twiddle
                double temp = u_real;
                u_real = u_real * w_real - u_imag * w_imag;
                u_imag = temp * w_imag + u_imag * w_real;
            }
        }
    }
    bit_reverse(real, imag, N); 
}
int main() {
    int N = 8;
    double real[8] = {1,1,1,1,0,0,0,0};
    double imag[8] = {0,0,0,0,0,0,0,0};

    FFT_DIF(real, imag, N);

    printf("=== DIF FFT Output ===\n");
    for (int i = 0; i < N; i++) {
        printf("X[%d] = %.4f + j%.4f\n", i, real[i], imag[i]);
    }
    return 0;
}
