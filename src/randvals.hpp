#ifndef __RANDVALS_DOT_HPP_GUARD__
#define __RANDVALS_DOT_HPP_GUARD__
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/math/distributions/pareto.hpp>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class RandomNumberGenerator {
    
    const uint64_t seed;
    boost::mt19937                  generator;
    boost::uniform_int< uint64_t >  uniIntDist;
    boost::variate_generator< boost::mt19937, boost::uniform_int< uint64_t > > randomU64;
    
public:
    RandomNumberGenerator( void ) :
    seed( time( NULL ) ),
    generator( seed ),
    uniIntDist( 0ULL, 0xFFFFFFFFFFFFFFFFULL ),
    randomU64( generator, uniIntDist )
    {}

    inline uint64_t RandU64( void ) {
        return randomU64();
    }
    inline uint64_t RandU64( const uint64_t max ) {
        return randomU64() % max;
    }
    inline uint64_t RandU64( const uint64_t min, const uint64_t max ) {
        const uint64_t d = max - min;
        const uint64_t v = randomU64() % d;
        return v + min;
    }
    void Seed( const uint64_t seed ) {
        randomU64.engine().seed( seed );
        randomU64.distribution().reset();
    }
};


#endif

