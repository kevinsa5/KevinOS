x = -3:0.005:3;
ex = exact(x);
for N = 1:8
    f = figure;
    hold all;
    y = zeros(size(x));
    for i = 1:length(x)
        y(i) = fourier1(x(i),N);
    end
    plot(x,y);
    plot(x,ex);
    plot(x,ex-y);
    legend('Approximation','Exact','Error');
    title(strcat('N = ', num2str(N)));
    print(f, '-r80', '-dtiff', strcat('image',num2str(N),'.tif'));
end
