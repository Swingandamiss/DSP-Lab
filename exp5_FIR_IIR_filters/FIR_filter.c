#include <stdio.h>
#include <math.h>

float x[100];
float XR[100];
float XI[100];
float b[51];
float y[100];
int N = 100;
float amplitude[100];
float amplitude1[100];
float phase[100];

float h[51] = {
};

int main(void) {
    int i, j, n;

    // Input signal
    for (i = 0; i < 100; i++) {
        x[i] = 0;
        x[i] = 5 + (2 * sin(2 * 3.14 * 30 * i / 100)) + (3 * cos(2 * 3.14 * 2 * i / 100));
    }

    // DFT before filtering
    for (j = 0; j < N; j++) {
        XR[j] = 0;
        XI[j] = 0;
        for (i = 0; i < N; i++) {
            XR[j] = XR[j] + (x[i] * cos(2 * 3.14 * i * j / N));
            XI[j] = XI[j] - (x[i] * sin(2 * 3.14 * i * j / N));
        }
    }

    for (i = 0; i < N; i++) {
        amplitude[i] = sqrt((XR[i] * XR[i]) + (XI[i] * XI[i]));
    }

    // FIR Filter
    for (n = 0; n < 100; n++) {
        y[n] = 0;
        b[0] = x[n];

        for (i = 51; i > 0; i--) {
            b[i] = b[i - 1];
        }

        for (i = 0; i < 51; i++) {
            y[n] = y[n] + (h[i] * b[i]);
        }
    }

    // DFT after filtering
    for (j = 0; j < N; j++) {
        XR[j] = 0;
        XI[j] = 0;
        for (i = 0; i < N; i++) {
            XR[j] = XR[j] + (y[i] * cos(2 * 3.14 * i * j / N));
            XI[j] = XI[j] - (y[i] * sin(2 * 3.14 * i * j / N));
        }
    }

    for (i = 0; i < N; i++) {
        amplitude1[i] = sqrt((XR[i] * XR[i]) + (XI[i] * XI[i]));
    }

    return 0;
}
