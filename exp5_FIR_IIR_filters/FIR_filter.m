fs = 8000;            % Sampling frequency
fc = 1000;            % Cutoff frequency
N = 15;               % Filter order
% Design FIR lowpass filter
b = fir1(N, fc/(fs/2));
% Example input: sum of two sines
n = 0:99;
x = sin(2*pi*200*n/fs) + sin(2*pi*1500*n/fs);
% Write coefficients to fir_coeffs.h
fid = fopen('fir_coeffs.h','w');
fprintf(fid, '#define FIR_LEN %d\n', length(b));
fprintf(fid, 'float fir_b[FIR_LEN] = {');
fprintf(fid, '%f, ', b(1:end-1));
fprintf(fid, '%f};\n', b(end));
fclose(fid);
% Write input to input_data.h
fid = fopen('input_data.h','w');
fprintf(fid, '#define INPUT_LEN %d\n', length(x));
fprintf(fid, 'float input_data[INPUT_LEN] = {');
fprintf(fid, '%f, ', x(1:end-1));
fprintf(fid, '%f};\n', x(end));
fclose(fid);
disp('Generated fir_coeffs.h and input_data.h');
