clear;
close all;

m  = 1024;
n  = 16;
l  = 1.0;

minload = 0;
maxload = 1.5;
numpts  = 400;
vl = linspace( minload, maxload, numpts );

params.fs = 16;       % font size
params.lw = 4;        % line width
params.fw = 'normal'; % font weight
params.resolution = '-r400';

% 1st figure: overflow versus number of ways
% 2nd figure: overflow versus table load
% vsways( m, vn, l, params );
[ vo, vt ] = loadtable( m, n, vl, params );

ltfn = sprintf( 'loadtable_%2d_ways.txt', n );
f = fopen( ltfn, 'w' );

vl = abs( vl );
vo = abs( vo );
vt = abs( vt );

fprintf( f, 'var vl = [ ' );
for idx = 1:(numpts-1)
    fprintf( f, '%f, ', vl(idx) );
end
fprintf( f, '%f ];\n', vl(numpts) );

fprintf( f, 'var vo = [ ' );
for idx = 1:(numpts-1)
    fprintf( f, '%f, ', vo(idx) );
end
fprintf( f, '%f ];\n', vo(numpts) );

fclose( f );
