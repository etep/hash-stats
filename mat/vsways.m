function vsways( m, vn, l, params )
    % ... single, insertions only
    % ... single, with deletions
    % ... 2-left, insertions only
    % ... 2-left, with deletions
    minways = min( vn );
    maxways = max( vn );
    
    figure;
    grid on;
    hold on;
    set( gca, 'FontSize',   params.fs );
    set( gca, 'FontWeight', params.fw );
    colors = winter(10);
    
    deletions = 1;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(3,:), 'LineWidth', params.lw );

    deletions = 0;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(3,:), 'LineWidth', params.lw, 'LineStyle', '--' );
    
    deletions = 1;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(9,:), 'LineWidth', params.lw );
    
    deletions = 0;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(9,:), 'LineWidth', params.lw, 'LineStyle', '--' );
    
    s = sprintf( 'table load = %.1f%%', 100*l );
    xlabel( 'ways per bucket' );
    ylabel( 'overflow usage' );
    title( s );
    axis([ minways maxways, 0, 50 ] );
    ytickslabel;
    legend( 'single, with deletions', 'single, insertions only', '2-left, with deletions', '2-left, insertions only', 'Location', 'NorthEast' );
    print( gcf, '-depsc2', params.resolution, 'zplotways' );
end


