
function [FDprob, meanLac, medianLac]  = getProbFD(img)
% GETPROBFD  Compute probability fractal dimension and lacunarity.
%
% Input:     img     a gray scale image (2D matrix, 0..255)
%
% Output:    FDprob  Probability Fractal Dimension
%            meanLac Mean Lacunarity over all box sizes
%
% Description:
% 
% The fractal dimension is estimated via the probability $P(m,L)$ to find a
% box of side length $L$ with $m$ points. The number of boxes to cover the
% image is then estimated via $N(L) = \sum_{m=1}^n P(m,L) / m$. A
% linear fit into the log-log plot of $N(L)$ vs. $L$ gives the fractal
% dimension. 
%
% Lacunarity can also be derived from $P(m,L)$:
% Let $M^q(L) = \sum_{m=1}^n m^q P(m,L)$, then the lacunarity is:
% $\Lambda(L) = M^2(L) / M(L)^2$ and the mean lacunarity is obtained by
% averaging over all L (similarly the median is returned). 
%
% This implementation avoids direct computation of P(m,L) and instead
% computes $M^q(L)$ directly for efficientcy (for q=-1,1,2). Also here we
% add 1 to the lacunarity as defined in Voss to be more consitent wit 
%
% See also:
% 
% Voss, R.: Random fractals: Characterization and measurement, in Scaling
% Phenomena in Disordered Systems, (R. Pynn and A. Skjeltorp, Eds.),
% Plenum, New York, (1986).
%
% Keller, J. M.; Chen, S.; Crownover, R. M.: Texture Description and
% Segmentation through Fractal Geometry, Computer Vision, Graphics and
% Image Processing 45, 150-166, (1989).
%
%
% Author:    Martin Reuter
% Copyright: Martin Reuter
% Date:      May , 2016
% License:   BSD 3-clause (see LICENSE)



% if color image is passed reduce to gray
if (size(img,3) > 1) 
    img = sum(img(:,:,1:3),3)/3.0;
end

% if range is 0..1, scale to 0 ..255:
if (max(img(:)) < 1)
    img = 255 * img;
end
img = uint8(img);
[h, w] = size(img);

lmax = min([w,h,31]);
if (mod(lmax,2) ==0)
    lmax= lmax-1;
end
border = (lmax-1)/2;
results = []; 
fprintf(1,'Calculating over h=%d and w=%d\n', h, w);
fprintf(1,'\n   l     log(l)        N(l)        -log(N(l))\n');
fprintf(1,'==================================================\n');
count = 0;
L = [];
NL = [];
LacL = [];
tic;
%PmL = [];
totalpix=(h-2*border)*(w-2*border);
for l=3:2:lmax
   count = count+1;
   r = (l-1) /2;
   Nsum = 0;
   Msum = 0;
   M2sum = 0;
   %PmL(count, :) = zeros(lmax*lmax,1);
   for y= border +1: h-border
       for x=border+1 : w-border    
        
           %iterate over box with radius r around x,y
           ibox = int16(img(y-r:y+r, x-r:x+r));
           ibox=abs(ibox-int16(img(y,x)))<=r;
           m = sum(ibox(:));
           Nsum = Nsum + 1.0/m;
           Msum = Msum + m;
           M2sum= M2sum +m^2;
           
           %PmL(count,m) = PmL(count,m) +1;
       end
   end
   %PmL(count,:) = PmL(count,:) / totalpix;
   fprintf(1,' %3d    %f   %12.6f    %10.6f  \n',l,log(l), Nsum, -log(Nsum));
   L(count) = l;
   NL(count) = Nsum /totalpix; %division does not change slope below
   M2sum = M2sum / totalpix;
   Msum = Msum / totalpix;
   %LacL(count) = (M2sum - Msum^2) / Msum^2;
   LacL(count) = M2sum  / Msum^2; % the above + 1, we'd add 1 anyway later
end
toc

results = [];
results(1,:) = log(L);
results(2,:) = -log(NL);
istart = 2;
iend = size(results,2)-0;
X = [ ones(iend-istart+1,1) results(1,istart:iend)'];
Y = results(2,istart:iend)';
[B,BINT,R,RINT, STATS] = regress(Y,X);

FDprob = B(2);
meanLac = mean(LacL(istart:iend));
medianLac = median(LacL(istart:iend));

fprintf('\nProbability Fractal Dimension : %0.5g\nMean Probability Lacunarity   : %0.5g\n',FDprob, meanLac);

return

% Plot linear fit
figure; hold on;
%plot(results(1,1:istart-1), results(2,1:istart-1), 'r.')
plot(results(1,istart:iend), results(2,istart:iend), 'b.')
%plot(results(1,iend+1:end), results(2,iend+1:end), 'r.')
Xfit = results(1,istart:iend);
Yfit = B(1) + B(2) * Xfit;
plot(Xfit,Yfit,':k');
xlabel('ln(L)');
ylabel('-ln(N)');
title(['Prob. Fractal Dim.: ' sprintf('%0.4g (corr: %0.3g)',FDprob,corr(Y,Yfit'))]);

figure; hold on;
plot(log(L(istart:iend)), log(LacL(istart:iend)), 'b.')
xlabel('ln(L)');
ylabel('ln(Lacunarity)');
title(['Prob. Lacunarity ' sprintf('(mean: %0.5g)',meanLac)]);

