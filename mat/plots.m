clear;
close all;

m  = 1024;
n  = 48;
l  = 1.0;

minways = 1;
maxways = 100;
minload = 0.8;
maxload = 1.4;
vn = minways:1:maxways;
vl = linspace( minload, maxload 100 );

params.fs = 16;       % font size
params.lw = 4;        % line width
params.fw = 'normal'; % font weight
params.resolution = '-r400';

% 1st figure: overflow versus number of ways
% 2nd figure: overflow versus table load
vsways( m, vn, l, params );
vsload( m, n, vl, params );

