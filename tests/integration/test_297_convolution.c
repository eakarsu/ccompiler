int printf(const char *fmt, ...);
// EXPECT: signal: 1 2 3 4 5 6\nkernel: 1 0 -1\nvalid conv: -2 -2 -2 -2\nfull conv: 1 2 2 2 2 2 -5 -6\nedge detect: -1 -1 -1 -1 -1\nsmoothed: 6 9 12 15\nmoving avg (w=3): 2 3 4 5\nimpulse response: 4 3 2
// Test: 1D convolution (signal processing)

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// 1D convolution: output[i] = sum(signal[i-j] * kernel[j]) for valid indices
// "valid" mode: output length = signal_len - kernel_len + 1
int convolve_valid(int *signal, int slen, int *kernel, int klen, int *output) {
    int olen = slen - klen + 1;
    int i; int j;
    for (i = 0; i < olen; i++) {
        int sum = 0;
        for (j = 0; j < klen; j++) {
            sum = sum + signal[i + j] * kernel[j];
        }
        output[i] = sum;
    }
    return olen;
}

// "full" mode: output length = signal_len + kernel_len - 1
int convolve_full(int *signal, int slen, int *kernel, int klen, int *output) {
    int olen = slen + klen - 1;
    int i; int j;
    for (i = 0; i < olen; i++) output[i] = 0;

    for (i = 0; i < slen; i++) {
        for (j = 0; j < klen; j++) {
            output[i + j] = output[i + j] + signal[i] * kernel[j];
        }
    }
    return olen;
}

// Moving average as convolution (integer division)
void moving_average(int *signal, int slen, int window, int *output) {
    int kernel[10];
    int i;
    for (i = 0; i < window; i++) kernel[i] = 1;

    int olen = slen - window + 1;
    for (i = 0; i < olen; i++) {
        int sum = 0;
        int j;
        for (j = 0; j < window; j++) {
            sum = sum + signal[i + j];
        }
        output[i] = sum / window;
    }
}

int main(void) {
    // Basic convolution
    int signal[6];
    signal[0]=1; signal[1]=2; signal[2]=3; signal[3]=4; signal[4]=5; signal[5]=6;
    int kernel[3];
    kernel[0]=1; kernel[1]=0; kernel[2]=-1;

    printf("signal: ");
    print_arr(signal, 6);
    printf("kernel: ");
    print_arr(kernel, 3);

    int valid_out[10];
    int vlen = convolve_valid(signal, 6, kernel, 3, valid_out);
    printf("valid conv: ");
    print_arr(valid_out, vlen);

    int full_out[20];
    int flen = convolve_full(signal, 6, kernel, 3, full_out);
    printf("full conv: ");
    print_arr(full_out, flen);

    // Edge detection kernel [1, -1]
    int edge_k[2]; edge_k[0]=1; edge_k[1]=-1;
    int edge_out[10];
    int elen = convolve_valid(signal, 6, edge_k, 2, edge_out);
    printf("edge detect: ");
    print_arr(edge_out, elen);

    // Smoothing kernel [1, 1, 1]
    int smooth_k[3]; smooth_k[0]=1; smooth_k[1]=1; smooth_k[2]=1;
    int smooth_out[10];
    int solen = convolve_valid(signal, 6, smooth_k, 3, smooth_out);
    printf("smoothed: ");
    print_arr(smooth_out, solen);

    // Moving average
    int ma_out[10];
    moving_average(signal, 6, 3, ma_out);
    printf("moving avg (w=3): ");
    print_arr(ma_out, 4);

    // Convolution with impulse [0,0,1,0,0]
    int impulse[5]; impulse[0]=0; impulse[1]=0; impulse[2]=1; impulse[3]=0; impulse[4]=0;
    int imp_k[3]; imp_k[0]=2; imp_k[1]=3; imp_k[2]=4;
    int imp_out[10];
    int ilen = convolve_valid(impulse, 5, imp_k, 3, imp_out);
    printf("impulse response: ");
    print_arr(imp_out, ilen);

    return 0;
}
