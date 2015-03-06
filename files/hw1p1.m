f = @(x,y) cos(2*pi*x)*sin(6*pi*y);

x = linspace(0,1,257);

z = zeros(257,257);
for i = 1:size(z,1)
    for j = 1:size(z,2)
        z(i,j) = f(x(i),x(j));
    end
end
surf(z,'EdgeColor','none');
title('Kevin Anderson');
