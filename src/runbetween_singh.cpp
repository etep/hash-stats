#include <stdint.h>

#include <set>
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
uint64_t u;
uint64_t maxload;
uint64_t maxu;

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
    if( (u-load) > (maxu-maxload) ) {
        maxu = u;
        maxload = load;
    }
    return tidx;
}

void printout( const uint64_t theu, const uint64_t theload, const char * title ) {
    const uint64_t keys_over = theu - theload;
    const double o = double( keys_over ) / (m*n);
    
    cout << title << ':' << endl;
    cout << "... keys inserted: " << theu    << endl;
    cout << "... keys in table: " << theload << endl;
    cout << "... keys overflow: " << keys_over << endl;
    cout << "... overflow pcnt: " << (100*o) << endl;
    cout << "..." << endl;
}

int main( int argc, char ** argv ) {
    
    double a = 1.0;
    
    const bool runInserts = true;
    const bool runDeletes = true;
    const bool bulkDelete = false;
    const bool runBetween = false;
    
    const uint64_t ngen = 2;
    const uint64_t ninserts = uint64_t( a * double( m*n ) );
    const uint64_t ndeletes = uint64_t( 0.15 * double( ninserts ) );
    
    table = new bool[ m*n ];
    indices = new uint64_t[ ninserts ];

    memset(   table, 0, m*n*sizeof( bool ) );
    memset( indices, 0, ninserts*sizeof( uint64_t ) );
    
    u = 0ULL;
    maxu = 0ULL;
    load = 0ULL;
    maxload = 0ULL;
    
    if( runInserts ) {
        printf( "running inserts...\n" );
        for( uint64_t i = 0ULL; i < ninserts; i++ ) {

            u++;
            indices[ i ] = Insert();
            
            if( i % (ninserts/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
            }
        }
        cout << "i =        n/a, u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
        printf( "...\n" );
    }
    
    if( runDeletes ) {
        printf( "running insert/delete pairs...\n" );
        for( uint64_t i = 0ULL; i < (m*n*ngen); i++ ) {
            
            // delete:
            const uint64_t delIdx = rng.RandU64() % u;
            Delete( indices[ delIdx ] );
            
            indices[ delIdx ] = Insert();

            if( i % (m*n/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
            }
        }
        cout << "i =        n/a, u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
        printf( "...\n" );
    }
    
    if( bulkDelete ) {
        printf( "running bulk delete...\n" );
        set< uint64_t > emptyIdxs;
        
        for( uint64_t i = 0ULL; i < ndeletes; i++ ) {
            
            // delete:
            uint64_t delIdx;
            do {
                delIdx = rng.RandU64() % u;
            } while( emptyIdxs.find( delIdx ) != emptyIdxs.end() );
            
            Delete( indices[ delIdx ] );
            emptyIdxs.insert( delIdx );
            
            u--;
            if( i % (ndeletes/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
            }
        }
        cout << "i =        n/a, u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
        printf( "...\n" );

        if( runBetween ) {
            printf( "running insert/delete pairs...\n" );
            for( uint64_t i = 0ULL; i < (m*n*ngen); i++ ) {
            
                // delete:
                uint64_t delIdx;
                do {
                    delIdx = rng.RandU64() % u;
                } while( emptyIdxs.find( delIdx ) != emptyIdxs.end() );
                Delete( indices[ delIdx ] );
        
                indices[ delIdx ] = Insert();

                if( i % (m*n/10) == 0ULL ) {
                    cout << "i = " << SETDEC( 10 ) << i << ", u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
                }
            }
            cout << "i =        n/a, u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
            printf( "...\n" );
        }
        
        uint64_t check = 0ULL;
        printf( "running resinsert...\n" );
        for( auto i : emptyIdxs ) {
            
            u++;
            check++;
            indices[ i ] = Insert();

            if( i % (ndeletes/10) == 0ULL ) {
                cout << "i = " << SETDEC( 10 ) << i << ", u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
            }
        }
        cout << "i =        n/a, u = " << SETDEC( 10 ) << u << ", load = " << SETDEC( 10 ) << load << endl;
        printf( "...\n" );
        assert( check == ndeletes );
    }
    
    printout(    u,    load, "endpoint load" );
    printout( maxu, maxload, "theworst load" );
}

