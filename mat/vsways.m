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
    colors = winter(4);
    
    deletions = 0;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(1,:), 'LineWidth', params.lw, 'LineStyle', '--' );
    
    deletions = 1;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = singh( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(3,:), 'LineWidth', params.lw, 'LineStyle', '--' );
    
    deletions = 0;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(2,:), 'LineWidth', params.lw );
    
    deletions = 1;
    vt = [];
    vo = [];
    for n = vn
        [ t, o ] = dleft( m, n, l, deletions );
        vt(end+1) = 100*t;
        vo(end+1) = 100*o;
    end
    plot( vn, vo, 'Color', colors(4,:), 'LineWidth', params.lw );
    
    s = sprintf( 'table load = %.1f%%', 100*l );
    xlabel( 'ways per bucket' );
    ylabel( 'overflow usage' );
    title( s );
    axis([ minways maxways, 0, 15 ] );
    ytickslabel;
    legend( 'single, insertions only', 'single, with deletions', '2-left, insertions only', '2-left, with deletions' );
    print( gcf, '-depsc2', params.resolution, 'zplotways' );
end


