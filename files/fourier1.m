function [ y ] = fourier1( x , N)
    series = zeros(1,N);
    for i= 1:N
        series(i) = 1/i^2 * (-1)^(i+1) * cos(i*pi*x/2);
    end
    y = 2/3 + 4/pi^2 * sum(series);
end

