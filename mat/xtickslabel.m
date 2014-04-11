xticks = get( gca, 'XTick' );
xtickv = {};
for xtick = xticks
    s = sprintf( '%3d%%', xtick );
    xtickv{ end+1 } = s;
end
set( gca, 'XTickLabel', xtickv );
