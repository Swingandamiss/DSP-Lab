#include <stdio.h>
#include <math.h>

#define NUM_SECTIONS 1

float b[NUM_SECTIONS][3] = { {0.0, 0.0214501370969255, 0.0} };   // filter coefficients
float a[NUM_SECTIONS][3] = { {1.0, -1.83233418096295, 0.859404279941073} }; // filter coefficients

float x[100];
int N = 100;
float XR[100];
float XI[100];
float m1[100];
float m2[100];
float y[100];
float w[NUM_SECTIONS][2] = {0}; // no of sections

int main(void) {
    int i;
    for (i = 0; i < N; i++) {
        x[i] = 0;
        x[i] = sin(2 * 3.14 * 2 * i / N) + sin(2 * 3.14 * 30 * i / N);
    }

    // DFT of input signal
    int j;
    for (j = 0; j < N; j++) {
        XR[j] = 0;
        XI[j] = 0;
        for (i = 0; i < N; i++) {
            XR[j] = XR[j] + (x[i] * cos(2 * 3.14 * i * j / N));
            XI[j] = XI[j] - (x[i] * sin(2 * 3.14 * i * j / N));
        }
    }

    for (i = 0; i < N; i++) {
        m1[i] = sqrt((XR[i] * XR[i]) + (XI[i] * XI[i]) / N);
    }

    // iirType equation here
    float input;
    float wn, yn;

    for (i = 0; i < N; i++) {
        input = x[i];
        int section;
        for (section = 0; section < NUM_SECTIONS; section++) {
            wn = input - a[section][1] * w[section][0] - a[section][2] * w[section][1];
            yn = b[section][0] * wn + b[section][1] * w[section][0] + b[section][2] * w[section][1];
            w[section][1] = w[section][0];
            w[section][0] = wn;
            input = yn;
        }
        y[i] = yn;
    }

    // DFT of Filtered signal
    for (j = 0; j < N; j++) {
        XR[j] = 0;
        XI[j] = 0;
        for (i = 0; i < N; i++) {
            XR[j] = XR[j] + (y[i] * cos(2 * 3.14 * i * j / N));
            XI[j] = XI[j] - (y[i] * sin(2 * 3.14 * i * j / N));
        }
    }

    for (i = 0; i < N; i++) {
        m2[i] = sqrt((XR[i] * XR[i]) + (XI[i] * XI[i]) / N);
    }

    return 0;
}
