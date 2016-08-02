% Author:    Martin Reuter
% Copyright: Martin Reuter
% Date:      May , 2016
% License:   BSD 3-clause (see LICENSE)

clear;
iname='~mreuter/Pictures/mike/553283_071.png'
iname='~mreuter/Pictures/mike/118667_060.png'

iname='path/to/my/file.png'

I = imread(iname);
figure; imshow(I,[])

% repliace tiles
[Itile,tile] = tileImg(I,0,200);
figure; imshow(Itile,[]);
figure; imshow(tile,[]);

% compute probability fractal dimension and mean lacunarity
[FDprob, meanLac] = getProbFD(Itile);

% compute capacity, information and correlation fractal dimension
[FDcap, FDinf, FDcor ] = getFD(Itile);

% we can also work on the small tile
[FDprob, meanLac] = getProbFD(tile);

