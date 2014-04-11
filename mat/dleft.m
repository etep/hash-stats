function [ t, o ] = dleft( m, n, l, x )
    % dleft.m: Calculates the utilization & number of overflows for a d-left (o.k., 2-left) hash table.
    % The overflows are reported as a percentage normalized to the table capacity.
    % For example, given a table with the following input parameters:
    % ...   m = 100 = number of buckets (input parameter)
    % ...   n =  10 = number of ways per bucket
    % ...   l = .95 = table load
    % ...   x =   1 = 1 runs insertions & deletions, 0 runs insertions only
    % There are 100 total entries in the table, and we are going to insert 95 unique elements. Some of
    % those will spill to an overflow list. The results are:
    % ...   t = .87 = 87 elements are stored in the table
    % ...   o = .08 =  8 elements spill to overflow
    % The probabilistic model used here assumes that there are a large number of buckets. By inspection,
    % these results would not hold if the number of buckets were one (of course having one bucket only
    % defeats the use of a hash function).
    %
    tic;
    
    %m = 100;    % hash buckets (essentially unused)
    %n = 10;     % hash ways
    %z = 4;      % over ways
    %l = 0.95;   % table load
    
    lpk = zeros( 1, n+1 );
    rpk = zeros( 1, n+1 );
    kiv = 0:1:n;
    kiv = fliplr( kiv );
    
    lpk( end ) = 0.5;
    rpk( end ) = 0.5;
    
    dt   = 0.0001;
    tend = 1.0;
    
    ch = l*n;
    u  = 0;
    
    % control:
    runinserts = 1;
    rundeletes = x;
    
    % inserts:
    if runinserts
        for t = dt:dt:tend
    
            dlpk = 4 * ch * diff( lpk );
            drpk = 4 * ch * diff( rpk );
    
            dlpk = dlpk .* rpk(2:end);
            dlpk( end+1 ) = 0;
            drpk( end+1 ) = 0;
            drpk = drpk .* lpk;
            
            lpk = lpk + dt*dlpk;
            rpk = rpk + dt*drpk;
            u = u + dt*ch;
        end
    else
        u = a*n;
    end
    
    % inserts & deletes:
    if rundeletes
        for t = dt:dt:(5*tend)
    
            dlpk = diff( lpk );
            drpk = diff( rpk );
            
            % deletion
            dlpk_dn = horzcat( lpk(1), dlpk(1:end-1), 0 );
            drpk_dn = horzcat( rpk(1), drpk(1:end-1), 0 );
            dlpk_dn = kiv .* dlpk_dn;
            drpk_dn = kiv .* drpk_dn;
            
            % insertion
            dlpk_up = 4 * ch * dlpk;
            drpk_up = 4 * ch * drpk;
            dlpk_up = dlpk_up .* rpk(2:end);
            dlpk_up( end+1 ) = 0;
            drpk_up( end+1 ) = 0;
            drpk_up = drpk_up .* lpk;
            
            dlpk = dlpk_up - dlpk_dn;
            drpk = drpk_up - drpk_dn;
            
            lpk = lpk + dt*dlpk;
            rpk = rpk + dt*drpk;
        end
    end
    
    lpk = diff( lpk );
    rpk = diff( rpk );
    
    kiv = 0:1:length(lpk)-1;
    kiv = fliplr( kiv );
    
    % 1. count buckets filled up to n-1: k_i * p_i
    % 2. count all buckets filled to n, or greater: n * sigma( p_i ) for k >= n
    % 3. overflows are the diff between ntable and total elements
    ntable = kiv * (lpk+rpk)';
    ntable = ntable + n*( 1 - sum(lpk) - sum(rpk ) );
    novers = u-ntable;
    
    o = novers / n;
    t = ntable / n;
    
    fprintf( 1, 'm = %6d buckets\n', m );
    fprintf( 1, 'n = %6d ways per bucket\n', n );
    fprintf( 1, 'l = %6.3f%%\n', 100*l );
    fprintf( 1, 't = %6.3f%%\n', 100*t );
    fprintf( 1, 'o = %6.3f%%\n', 100*o );
    
    toc;
end





