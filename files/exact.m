function [ y ] = exact( x )
    y = zeros(1,length(x));
    for i = 1:length(x)
        if x(i) < -2
            x(i) = x(i) + 4;
        elseif x(i) > 2
            x(i) = x(i) - 4;
        end
        y(i) = 1-x(i)^2/4;
    end
    
end

