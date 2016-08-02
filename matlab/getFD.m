function [ FDcap, FDinf, FDcor ] = getFD(img)
% GETFD      Compute fractal dimensions.
%
% Input:     img     a gray scale image (2D matrix, 0..255)
%
% Output:    FDcap   Capacity Fractal Dimension (box counting)
%            FDinf   Information Fractal Dimension
%            FDcor   Correlation Fractal Dimension
%
%
% Author:    Martin Reuter
% Copyright: Martin Reuter
% Date:      May , 2016
% License:   BSD 3-clause (see LICENSE)
%


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

lmax = min([w,h,64]);

wn = floor(w / lmax) * lmax;
hn = floor(h / lmax) * lmax;


fprintf(1,'Calculating over h=%d and w=%d\n', hn, wn);
L = [];
NL = [];
InfL = [];
SqrFreqL = [];
count = 0;
hnwn = hn*wn;
loghw = log(hnwn);
for boxsize = pow2(1:floor(log2(lmax)))
    Nsum = 0;
    i = 0;
    Inf = 0;
    SqrFreq = 0;
    bs2 = boxsize*boxsize;
    count = count+1;
    for (k = 1:boxsize:hn-boxsize+1)
      for (l = 1:boxsize:wn-boxsize+1)
        ibox = img(k:k+boxsize-1,l:l+boxsize-1);
        maxi = single(max(ibox(:)));
        mini = single(min(ibox(:)));
        %fprintf('max %d  min %d   bs %d  add %d\n',maxi,mini,boxsize,floor((maxi - mini) / boxsize)+1);
        N = floor((maxi - mini) / boxsize)+1;  %number of boxes (round up)
        Nsum = Nsum+ N; 
        Inf = Inf + bs2 * (log(bs2 / N) - loghw );
        SqrFreq = SqrFreq + bs2 *bs2 / N;
        i = i+1;    % count squares
      end
    end
    fprintf('Quadrate: %5d ', i);
    fprintf('N=%7.1f and -ln(N)=%10.6f for boxsize %3d\n', Nsum, -log(single(Nsum)), boxsize);
    L(count) = boxsize;
    NL(count) = Nsum;
    InfL(count) = Inf / hnwn;
    SqrFreqL(count) = SqrFreq / hnwn^2;
end

results = [];
results(1,:) = log(L);
results(2,:) = -log(NL);
results(2,:) = InfL;
results(2,:) = log(SqrFreqL);

istart = 2;
iend = size(results,2)-0;
X = [ ones(iend-istart+1,1) results(1,istart:iend)'];

results(2,:) = -log(NL);
Y = results(2,istart:iend)';
[B,BINT,R,RINT, STATS] = regress(Y,X);
FDcap = B(2);

results(2,:) = InfL;
Y = InfL(istart:iend)';
[B,BINT,R,RINT, STATS] = regress(Y,X);
FDinf = B(2);

results(2,:) = log(SqrFreqL);
Y = results(2,istart:iend)';
[B,BINT,R,RINT, STATS] = regress(Y,X);
FDcor = B(2);

fprintf('\nCapacity Fractal Dimension    : %0.5g\nInformation Fractal Dimension : %0.5g\nCorrelation Fractal Dimension : %0.5g\n',FDcap,FDinf, FDcor);

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
title(['Prob. Fractal Dim.: ' sprintf('%0.4g (corr: %0.3g)',FD,corr(Y,Yfit'))]);
