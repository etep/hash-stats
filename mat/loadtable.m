function [ vo, vt ] = loadtable( m, n, vl, params )
    % ... single, insertions only
    % ... single, with deletions
    % ... 2-left, insertions only
    % ... 2-left, with deletions
    minload = min( vl );
    maxload = max( vl );
    
    figure;
    grid on;
    hold on;
    set( gca, 'FontSize',   params.fs );
    set( gca, 'FontWeight', params.fw );
    colors = winter(10);
        
    deletions = 1;
    vt = [];
    vo = [];
    for l = vl
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( 100*vl, vo, 'Color', colors(3,:), 'LineWidth', params.lw );
    
    legend( 'single', 'Location', 'SouthEast' );
    vo = vo/100;
    vl = vl/100;
end



