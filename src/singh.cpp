#include <stdint.h>
#include <iostream>

#include <randvals.hpp>

#define SETFIX( w, p )   std::fixed      << std::setprecision( p ) << std::setw( w ) << std::setfill(' ')
#define SETSCI( w )      std::scientific << std::setprecision( w )
#define SETDEC( w )   std::dec   << std::setw( w ) << std::setfill(' ')
#define SETHEX( w )   std::hex   << std::setw( w ) << std::setfill('0')
#define ZERODEC( w )  std::dec   << std::setw( w ) << std::setfill('0')

using namespace std;

static const uint32_t idxBits = 18;
static const uint64_t m = 1ULL << idxBits;
uint64_t n = 10;

bool * table;
uint64_t * indices;

uint64_t load;
uint64_t rload;

RandomNumberGenerator rng;

void Delete( uint64_t idx ) {
    if( idx < m*n ) {
        table[ idx ] = false;
        assert( load );
        load--;
    }
}

uint64_t Insert( void ) {
    
    const uint64_t key = rng.RandU64();
    
    const uint32_t bucket = key % m;
    
    const uint32_t begIdx = 0 + bucket*n;
    const uint32_t endIdx = n + bucket*n;
    
    uint32_t occ = 0;
    uint32_t tidx = 0xFFFFFFFF;

    for( uint32_t idx = begIdx; idx < endIdx; idx++ ) {

        const bool val = table[ idx ];
        bool found = false;

        if( val ) {
            occ++;
        }
        else if( !found ) {
            tidx = idx;
            found = true;
        }
    }
    
    if( occ == n ) {
        // overflow
        assert( tidx == 0xFFFFFFFF );
        return m*n;
    }

    assert( tidx != 0xFFFFFFFF );
    assert( table[ tidx ] == false );
    table[ tidx ] = true;
    load++;
    return tidx;    
}

int main( int argc, char ** argv ) {
    
    double a = 0.8;
    
    const bool runInserts = true;
    bool runDeletes = false;
    
    const uint64_t ngen = 2;
    const uint64_t ninserts = uint64_t( a * double( m*n ) );
    
    table = new bool[ m*n ];
    indices = new uint64_t[ m*n ];

    memset(   table, 0, m*n*sizeof( bool ) );
    memset( indices, 0, m*n*sizeof( uint64_t ) );
    
    load = 0ULL;
    uint64_t u = 0ULL;

    if( runInserts ) {
        for( uint64_t i = 0ULL; i < ninserts; i++ ) {

            indices[ i ] = Insert();
        
            if( i % (ninserts/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", load = " << load << endl;
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
                cout << "i = " << SETDEC( 10 ) << i << ", load = " << load << endl;
            }
        }
    }
    
    const uint64_t keys_over = u - load;
    const double o = double( keys_over ) / (m*n);
    
    cout << "keys inserted: " << u << endl;
    cout << "keys in table: " << load << endl;
    cout << "keys overflow: " << keys_over << endl;
    cout << "overflow pcnt: " << (100*o) << endl;
}

