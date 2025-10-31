#include <stdio.h>
#include <stdlib.h>
void linear_convolution(float *x, int x_len, float *h, int h_len, float *y) {
    int y_len = x_len + h_len - 1;
    int n;
    for (n = 0; n < y_len; n++) {
        y[n] = 0.0;
        int k;
        for (k = 0; k < h_len; k++) {
            if (n - k >= 0 && n - k < x_len) {
                y[n] += x[n - k] * h[k];
            }
        }
    }
}
void circular_convolution(float *x, float *h, float *y, int N) {
    int n;
    for (n = 0; n < N; n++) {
        y[n] = 0.0;
        int k;
        for (k = 0; k < N; k++) {
            int index = (n - k + N) % N;
            y[n] += x[k] * h[index];
        }
    }
}
void print_array(float *arr, int len, const char *label) {
    printf("%s:\n", label);
    int i;
    for (i = 0; i < len; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n\n");
}
int main() {
    float x[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    float h[] = {2.0, 1.0, 2.0, 1.0, 2.0, 1.0, 2.0, 1.0};
    int x_len = sizeof(x) / sizeof(x[0]);
    int h_len = sizeof(h) / sizeof(h[0]);
    int y_linear_len = x_len + h_len - 1;
    float *y_linear = (float *)malloc(sizeof(float) * y_linear_len);
    if (y_linear == NULL) return -1;
    int N = (x_len > h_len) ? x_len : h_len;
    float *x_pad = (float *)calloc(N, sizeof(float));
    float *h_pad = (float *)calloc(N, sizeof(float));
    float *y_circular = (float *)malloc(sizeof(float) * N);
    if (x_pad == NULL || h_pad == NULL || y_circular == NULL) return -1;
    int i;
    for (i = 0; i < N; i++) {
        x_pad[i] = (i < x_len) ? x[i] : 0.0;
        h_pad[i] = (i < h_len) ? h[i] : 0.0;
    }
    linear_convolution(x, x_len, h, h_len, y_linear);
    circular_convolution(x_pad, h_pad, y_circular, N);
    printf("y_circular address: 0x%p\n", (void*)y_circular);
    print_array(x, x_len, "Input x[n]");
    print_array(h, h_len, "Impulse Response h[n]");
    print_array(y_linear, y_linear_len, "Linear Convolution Output");
    print_array(y_circular, N, "Circular Convolution Output");
    free(y_linear);
    free(x_pad);
    free(h_pad);
    free(y_circular);
    return 0;
}
