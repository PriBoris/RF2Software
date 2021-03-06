
x = [0:0.001:1];

z = zeros(1, length(x));

y1 = ones(1, length(x));

k2 = (1/0.5)^2;
y2 = 1-k2*(x-0.5).^2;

k4 = (1/0.5)^4;
y4 = 1-k4*(x-0.5).^4;

k6 = (1/0.5)^6;
y6 = 1-k6*(x-0.5).^6;

k8 = (1/0.5)^8;
y8 = 1-k8*(x-0.5).^8;

y = [y1;y2;y4;y6;y8];

plot(x, y, x, z,'k')
grid on


