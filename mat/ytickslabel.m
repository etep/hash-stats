yticks = get( gca, 'YTick' );
ytickv = {};
for ytick = yticks
    s = sprintf( '%3d%%', ytick );
    ytickv{ end+1 } = s;
end
set( gca, 'YTickLabel', ytickv );
