function [Itile, tile, cropped] = tileImg(img, mask, wo, ho)
% TILEIMG    Find largest rectangle (=tile) in interior and replicate
%
% Input:     img      a gray scale image (2D matrix, 0..255)
%            mask     a background value (default 0) or a binary mask
%            wo       output image desired width  (default 100)
%            ho       output image desired height (default 100)
%
% Output:    Itile    output image (wo x ho) made out of tiles
%            tile     a single tile (largest rectangle inside mask)
%            cropped  smallest rectangle containing full mask
%
% Description:
%
% TILEIMG will find the largest rectangle inside the masked interior of an
% image (img). The mask can either be a scalar value of the background
% intenstiy (usually 0) or a binary image of the same size as the input
% image with values 0=background and 1=foreground. 
% 
% The algorithm will first find the smallest bounding box around the
% foreground component (cropped) and will then find the largest rectangle
% in the interior of the masked foreground (tile). The algorithm is not
% guaranteed to be optimal for ill shaped domains, but works well for
% convex domains (circular, elliptical). Finally the tile is iteratively 
% flipped (left/right and also up/down) to fill the full target image Itile
% of dimensions height ho and width wo as passed.
%
%
% Author:    Martin Reuter
% Copyright: Martin Reuter
% Date:      May , 2016
% License:   BSD 3-clause (see LICENSE)
%

% create mask from bgcolor=0
if ~exist('mask', 'var') || isempty(mask)
    bgcolor = 0;
    mask = ones(size(img));
    mask(find(img==bgcolor)) = 0;
end

% if mask is scalar, create mask from bgcolor scalar
if length(mask(:)) == 1
    bgcolor = mask;
    mask = ones(size(img));
    mask(find(img==bgcolor)) = 0;
end

% output dimensions
if ~exist('wo', 'var') || isempty(wo)
    wo = 100;
end
if ~exist('ho', 'var') || isempty(ho)
    ho = wo;
end

%crop image and mask to bounding box
xproj = max(mask,[],1);
yproj = max(mask,[],2);
xidx = find(xproj > 0);
yidx = find(yproj > 0);
cropped = img(min(yidx):max(yidx),min(xidx):max(xidx));
mask = mask(min(yidx):max(yidx),min(xidx):max(xidx));
%figure; imshow(cropped,[]);

% find largest rectangle in interior by shrinking sides
% (note for ugly shapes this may not find the largest region, but 
% works well for convex shapes)
[h,w] = size(mask);
% box has up-left-row, up-left-col, low-right-row, low-right-col
box = [1 1 h w ];
params = 1:4;
pid = 0;
% shrink box
while ~isempty(params)
    % switch to next parameter
    pid = mod( pid, length(params) ) + 1;
    p = params(pid); 
    % update current parameter: + 1 top-left (1,2) , -1 for low-right (3,4)
    box(p) = box(p) + sign(2.5-p);
    % grab row or column
    if (p==1 || p==3); region = mask(box(p),box(2):box(4)); end;
    if (p==2 || p==4); region = mask(box(1):box(3),box(p)); end;
    % if row/col is completely inside, stop shrinking this param
    if isempty(find(region==0,1))
        params(pid) = [];
    end
end
% box now contains the 4 coords to create the tile:
tile = cropped(box(1):box(3), box(2):box(4));
%figure; imshow(tile,[]);

% to avoid boundary edges at tile intersections, flip tiles
fulltile = [ tile fliplr(tile) ];
fulltile = [ fulltile ; flipud(fulltile) ] ;
%fulltile = tile;  % set this if you don't want the mirroring

% tile to fill full target
[h,w] = size(fulltile);
Itile = repmat(fulltile,ceil(ho/h),ceil(wo/w));
Itile = Itile(1:ho,1:wo);
%figure; imshow(Itile,[]);

