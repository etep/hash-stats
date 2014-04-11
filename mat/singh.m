function [ t, o ] = singh( m, n, l, x )
    % singh.m: Calculates the utilization & number of overflows for a hash table.
    % The overflows are reported as a percentage normalized to the table capacity.
    % For example, given a table with the following input parameters:
    % ...   m = 100 = number of buckets (input parameter)
    % ...   n =  10 = number of ways per bucket
    % ...   l = .95 = table load
    % ...   x =   1 = 1 runs insertions & deletions, 0 runs insertions only
    % There are 100 total entries in the table, and we are going to insert 95 unique elements. Some of
    % those will spill to an overflow list. The results are:
    % ...   t = .77 = 77 elements are stored in the table
    % ...   o = .18 = 18 elements spill to overflow
    % The probabilistic model used here assumes that there are a large number of buckets. By inspection,
    % these results would not hold if the number of buckets were one (of course having one bucket only
    % defeats the use of a hash function).
    %
    tic;
    
    pk  = zeros( 1, n+1 );
    kiv = 0:1:n;
    kiv = fliplr( kiv );
    
    pk( end ) = 1.0;

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
    
            dpk = ch * diff( pk );
            dpk( end+1 ) = 0;

            pk = pk + dt*dpk;
            u = u + dt*ch;
        end
    else
        u = a*n;
    end
    
    % inserts & deletes:
    if rundeletes
        for t = dt:dt:(5*tend)
    
            dpk = diff( pk );

            % deletion
            dpk_dn = horzcat( pk(1), dpk(1:end-1), 0 );
            dpk_dn = kiv .* dpk_dn;

            % insertion
            dpk_up = ch * dpk;
            dpk_up( end+1 ) = 0;
            
            dpk = dpk_up - dpk_dn;
            pk  = pk + dt*dpk;
        end
    end
    
    pk = diff( pk );

    kiv = 0:1:length(pk)-1;
    kiv = fliplr( kiv );
    
    % 1. count buckets filled up to n-1: k_i * p_i
    % 2. count all buckets filled to n, or greater: n * sigma( p_i ) for k >= n
    % 3. overflows are the diff between ntable and total elements
    ntable = kiv * pk';
    ntable = ntable + n*( 1 - sum(pk) );
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





