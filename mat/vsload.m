function vsload( m, n, vl, params )
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
    plot( vo, vt, 'Color', colors(3,:), 'LineWidth', params.lw );
    
    deletions = 0;
    vt = [];
    vo = [];
    for l = vl
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vo, vt, 'Color', colors(3,:), 'LineWidth', params.lw, 'LineStyle', '--' );
        
    deletions = 1;
    vt = [];
    vo = [];
    for l = vl
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vo, vt, 'Color', colors(9,:), 'LineWidth', params.lw );
    
    deletions = 0;
    vt = [];
    vo = [];
    for l = vl
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vo, vt, 'Color', colors(9,:), 'LineWidth', params.lw, 'LineStyle', '--' );
    
    s = sprintf( 'ways per bucket = %d', n );
    ylabel( 'table utilization achieved' );
    xlabel( 'overflow provision' );
    title( s );
    axis([ -.1 40, 80, 100.1 ]);
    set( gca, 'XTick', 0:5:40 );
    ytickslabel;
    xtickslabel;
    legend( 'single, with deletions', ...
            'single, insertions only', ...
            '2-left, with deletions', ...
            '2-left, insertions only', ...
            'Location', 'SouthEast' );
    print( gcf, '-depsc2', params.resolution, 'zplotload' );
end



