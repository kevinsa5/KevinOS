k = 1.04;
p = 10.6;
s = 0.056;
c = sqrt(k/(p*s));
L = 10;


for t = [1,2,3,10,50]
    u = 50;
    du = 0;
    for n = 1:50
        du = du + (-1)^(n+1)/n * sin(n*pi/2) * exp(-(n*pi*c/L)^2*t);
    end
    u = u + 200/pi * du;
    disp(strcat('t = ', num2str(t), ' : u(L/2) = ', num2str(u)));
end
