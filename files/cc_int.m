function I = clenshaw_curtis(f,n)   % (n+1)-pt C-C quadrature of f
    x = cos(pi*(0:n)'/n);               % Chebyshev points
    fx = feval(f,x)/(2*n);              % f evaluated at these points
    g = real(fft(fx([1:n+1 n:-1:2])));  % Fast Fourier Transform
    a = [g(1); g(2:n)+g(2*n:-1:n+2); g(n+1)]; % Chebyshev coefficients
    w = 0*a'; w(1:2:end) = 2./(1-(0:2:n).^2); % weight vector
    I = w*a;                            % the integral
end
