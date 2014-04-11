#include <stdint.h>
#include <iostream>

#include <randvals.hpp>

#define SETFIX( w, p )   std::fixed      << std::setprecision( p ) << std::setw( w ) << std::setfill(' ')
#define SETSCI( w )      std::scientific << std::setprecision( w )
#define SETDEC( w )   std::dec   << std::setw( w ) << std::setfill(' ')
#define SETHEX( w )   std::hex   << std::setw( w ) << std::setfill('0')
#define ZERODEC( w )  std::dec   << std::setw( w ) << std::setfill('0')

using namespace std;

static const uint32_t idxBits = 15;
static const uint64_t m = 1ULL << idxBits;
static const uint64_t n = 10;
static const double l = 0.95;

bool * l_table;
bool * r_table;
uint64_t * indices;

uint64_t lload;
uint64_t rload;

RandomNumberGenerator rng;

void Delete( uint64_t idx ) {
    if( idx < m*n/2 ) {
        // left table
        l_table[ idx ] = false;
        assert( lload );
        lload--;
    }
    else if( idx < m*n ) {
        // right table
        idx -= (m*n/2);
        r_table[ idx ] = false;
        assert( rload );
        rload--;
    }
}

uint64_t Insert( void ) {
    
    const uint64_t lkey = rng.RandU64();
    const uint64_t rkey = rng.RandU64();
    
    const uint32_t lbucket = lkey % (m/2);
    const uint32_t rbucket = rkey % (m/2);
    
    const uint32_t lbegIdx = 0 + lbucket*n;
    const uint32_t lendIdx = n + lbucket*n;
    const uint32_t rbegIdx = 0 + rbucket*n;
    const uint32_t rendIdx = n + rbucket*n;
    
    uint32_t locc = 0;
    uint32_t rocc = 0;
    uint32_t lidx = 0xFFFFFFFF;
    uint32_t ridx = 0xFFFFFFFF;
    
    for( uint32_t idx = lbegIdx; idx < lendIdx; idx++ ) {

        const bool lval = l_table[ idx ];
        bool lfound = false;

        if( lval ) {
            locc++;
        }
        else if( !lfound ) {
            lidx = idx;
            lfound = true;
        }
    }
    for( uint32_t idx = rbegIdx; idx < rendIdx; idx++ ) {

        const bool rval = r_table[ idx ];
        bool rfound = false;

        if( rval ) {
            rocc++;
        }
        else if( !rfound ) {
            ridx = idx;
            rfound = true;
        }
    }
    
    if( locc == n && rocc == n ) {
        // overflow
        assert( lidx == 0xFFFFFFFF );
        assert( ridx == 0xFFFFFFFF );
        return m*n;
    }
    if( rocc < locc ) {
        // right table is less loaded
        assert( ridx != 0xFFFFFFFF );
        r_table[ ridx ] = true;
        rload++;
        return ridx + m*n/2;
    }
    
    // either left table is less loaded, or it is a tie
    assert( lidx != 0xFFFFFFFF );
    l_table[ lidx ] = true;
    lload++;
    return lidx;    
}

int main( int argc, char ** argv ) {
    const bool runInserts = true;
    const bool runDeletes = true;
    
    const uint64_t ngen = 2;
    const uint64_t ninserts = uint64_t( l * double(m*n) );
    
    l_table = new bool[ m*n/2 ];
    r_table = new bool[ m*n/2 ];
    indices = new uint64_t[ ninserts ];

    memset( l_table, 0, m*n*sizeof( bool )/2 );
    memset( r_table, 0, m*n*sizeof( bool )/2 );
    memset( indices, 0, ninserts*sizeof( uint64_t ) );
    
    lload = 0ULL;
    rload = 0ULL;
    uint64_t u = 0ULL;

    if( runInserts ) {
        for( uint64_t i = 0ULL; i < ninserts; i++ ) {

            indices[ i ] = Insert();
        
            if( i % (m*n/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", load = " << (lload + rload) << endl;
            }
            u++;
        }
    }
    
    if( runDeletes ) {
        for( uint64_t i = 0ULL; i < (m*n*ngen); i++ ) {
            
            // delete:
            const uint64_t delIdx = rng.RandU64() % u;
            Delete( indices[ delIdx ] );
        
            indices[ delIdx ] = Insert();

            if( i % (m*n/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", load = " << (lload + rload) << endl;
            }
        }
    }
    
    const uint64_t keys_over = u - lload - rload;
    const double o = double( keys_over ) / (m*n);
    
    cout << "keys inserted: " << u << endl;
    cout << "keys in table: " << (lload+rload) << endl;
    cout << "keys overflow: " << keys_over << endl;
    cout << "overflow pcnt: " << (100*o) << endl;
}

