/*
 * ctmf.c - Constant-time median filtering
 * Copyright (C) 2006  Simon Perreault
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * Contact:
 *  Laboratoire de vision et syst¿ßmes num­Triques
 *  Pavillon Adrien-Pouliot
 *  Universit­T Laval
 *  Sainte-Foy, Qu­Tbec, Canada
 *  G1K 7P4
 *
 *  perreaul@gel.ulaval.ca
 */


/* Standard C includes */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Type declarations */
#ifdef _MSC_VER
#include <basetsd.h>
//typedef UINT8  uint8_t;
//typedef UINT16 uint16_t;
//typedef UINT32 uint32_t;
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned int  uint32_t;

#pragma warning( disable: 4799 )
#else
#include <stdint.h>
#endif

/* Intrinsic declarations */
#if defined(__SSE2__) || defined(__MMX__)
#if defined(__SSE2__)
#include <emmintrin.h>
#elif defined(__MMX__)
#include <mmintrin.h>
#endif
#if defined(__GNUC__)
#include <mm_malloc.h>
#elif defined(_MSC_VER)
#include <malloc.h>
#endif
#elif defined(__ALTIVEC__)
#include <altivec.h>
#endif

/* Compiler peculiarities */
#if defined(__GNUC__)
#include <stdint.h>
#define inline __inline__
#define align(x) __attribute__ ((aligned (x)))
#elif defined(_MSC_VER)
#define inline __inline
#define align(x) __declspec(align(x))
#else
#define inline
#define align(x)
#endif

/**
 * This structure represents a two-tier histogram. The first tier (known as the
 * "coarse" level) is 4 bit wide and the second tier (known as the "fine" level)
 * is 8 bit wide. Pixels inserted in the fine level also get inserted into the
 * coarse bucket designated by the 4 MSBs of the fine bucket value.
 *
 * The structure is aligned on 16 bits, which is a prerequisite for SIMD
 * instructions. Each bucket is 16 bit wide, which means that extra care must be
 * taken to prevent overflow.
 */
typedef struct align{
    uint16_t coarse[16];
    uint16_t fine[256];
} Histogram;

/**
 * HOP is short for Histogram OPeration. This macro makes an operation \a op on
 * histogram \a h for pixel value \a i. It takes care of handling both levels.
 */
#define HOP(h,i,op) \
    h.coarse[i>>4] op; \
    h.fine[i] op;

/**
 * Adds histograms \a x and \a y and stores the result in \a y. Makes use of
 * SSE2, MMX or Altivec, if available.
 */
#if defined(__SSE2__)
static inline void histogram_add( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] += x->coarse[i];
            *(__m128i*) &y->fine[16*i] =
                _mm_add_epi16( *(__m128i*) &y->fine[16*i], *(__m128i*) &x->fine[16*i] );
            *(__m128i*) &y->fine[16*i+8] =
                _mm_add_epi16( *(__m128i*) &y->fine[16*i+8], *(__m128i*) &x->fine[16*i+8] );
        }
    }
}
#elif defined(__MMX__)
static inline void histogram_add( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] += x->coarse[i];
            *(__m64*) &y->fine[16*i] =
                _mm_add_pi16( *(__m64*) &y->fine[16*i], *(__m64*) &x->fine[16*i] );
            *(__m64*) &y->fine[16*i+4] =
                _mm_add_pi16( *(__m64*) &y->fine[16*i+4], *(__m64*) &x->fine[16*i+4] );
            *(__m64*) &y->fine[16*i+8] =
                _mm_add_pi16( *(__m64*) &y->fine[16*i+8], *(__m64*) &x->fine[16*i+8] );
            *(__m64*) &y->fine[16*i+12] =
                _mm_add_pi16( *(__m64*) &y->fine[16*i+12], *(__m64*) &x->fine[16*i+12] );
        }
    }
}
#elif defined(__ALTIVEC__)
static inline void histogram_add( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] += x->coarse[i];
            *(vector unsigned short*) &y->fine[16*i] =
                vec_add( *(vector unsigned short*) &y->fine[16*i],
                         *(vector unsigned short*) &x->fine[16*i] );
            *(vector unsigned short*) &y->fine[16*i+8] =
                vec_add( *(vector unsigned short*) &y->fine[16*i+8],
                         *(vector unsigned short*) &x->fine[16*i+8] );
        }
    }
}
#else
static inline void histogram_add( const Histogram* x, Histogram* y )
{
    int i, j;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] += x->coarse[i];
            for ( j = 16*i; j < 16*(i+1); ++j ) {
                y->fine[j] += x->fine[j];
            }
        }
    }
}
#endif

/**
 * Subtracts histogram \a x from \a y and stores the result in \a y. Makes use
 * of SSE2, MMX or Altivec, if available.
 */
#if defined(__SSE2__)
static inline void histogram_sub( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] -= x->coarse[i];
            *(__m128i*) &y->fine[16*i] =
                _mm_sub_epi16( *(__m128i*) &y->fine[16*i], *(__m128i*) &x->fine[16*i] );
            *(__m128i*) &y->fine[16*i+8] =
                _mm_sub_epi16( *(__m128i*) &y->fine[16*i+8], *(__m128i*) &x->fine[16*i+8] );
        }
    }
}
#elif defined(__MMX__)
static inline void histogram_sub( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] -= x->coarse[i];
            *(__m64*) &y->fine[16*i] =
                _mm_sub_pi16( *(__m64*) &y->fine[16*i], *(__m64*) &x->fine[16*i] );
            *(__m64*) &y->fine[16*i+4] =
                _mm_sub_pi16( *(__m64*) &y->fine[16*i+4], *(__m64*) &x->fine[16*i+4] );
            *(__m64*) &y->fine[16*i+8] =
                _mm_sub_pi16( *(__m64*) &y->fine[16*i+8], *(__m64*) &x->fine[16*i+8] );
            *(__m64*) &y->fine[16*i+12] =
                _mm_sub_pi16( *(__m64*) &y->fine[16*i+12], *(__m64*) &x->fine[16*i+12] );
        }
    }
}
#elif defined(__ALTIVEC__)
static inline void histogram_sub( const Histogram* x, Histogram* y )
{
    int i;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] -= x->coarse[i];
            *(vector unsigned short*) &y->fine[16*i] =
                vec_sub( *(vector unsigned short*) &y->fine[16*i],
                         *(vector unsigned short*) &x->fine[16*i] );
            *(vector unsigned short*) &y->fine[16*i+8] =
                vec_sub( *(vector unsigned short*) &y->fine[16*i+8],
                         *(vector unsigned short*) &x->fine[16*i+8] );
        }
    }
}
#else
static inline void histogram_sub( const Histogram* x, Histogram* y )
{
    int i, j;
    for ( i = 0; i < 16; ++i ) {
        if ( x->coarse[i] ) {
            y->coarse[i] -= x->coarse[i];
            for ( j = 16*i; j < 16*(i+1); ++j ) {
                y->fine[j] -= x->fine[j];
            }
        }
    }
}
#endif

/**
 * Computes a histogram's median value, for filter radius \a r.
 */
static inline uint8_t histogram_median( const Histogram* h, uint32_t r )
{
    const uint16_t m = 2*r*r + 2*r;
    uint16_t sum = 0;
    uint32_t i;

    /*
     * Cumulative sum for coarse level. Stops when the total meets a threshold,
     * then backtracks one bucket.
     */
    for ( i = 0; i < 16; ++i ) {
        sum += h->coarse[i];
        if ( sum > m ) {
            sum -= h->coarse[i];
            break;
        }
    }

    /*
     * Then I find the median bucket at the fine level. On average, I need 16
     * comparisons instead of 128 if I had only one level.
     */
    for ( i *= 16; i < 256; ++i ) {
        sum += h->fine[i];
        if ( sum > m ) {
            return (uint8_t) i;
        }
    }

    /* Shouldn't be here... */
    assert(0);
    return 0;
}

static void ctmf_helper( const unsigned char* const src, unsigned char* const dst,
        const int width, const int height,
        const int src_step_row, const int src_step_col, const int dst_step_row, const int dst_step_col,
        const int r, const int pad_left, const int pad_right )
{
    const int m = height, n = width;
    int i, j;

    Histogram *h, H;

#ifdef __SSE2__
    /* This mask selects the 4 lower bytes of 16 8-bit elements. */
    const __m128i mask = _mm_set_epi32( 0x0F0F0F0F, 0x0F0F0F0F, 0x0F0F0F0F, 0x0F0F0F0F );
    __m128i buffer, shifted;
#endif

    assert( src );
    assert( dst );
    assert( r >= 0 );
    assert( width >= 2*r+1 );
    assert( height >= 2*r+1 );
    assert( src_step_row != 0 );
    assert( src_step_col != 0 );
    assert( dst_step_row != 0 );
    assert( dst_step_col != 0 );

    /* SSE2 and MMX need aligned memory, provided by _mm_malloc(). */
#if defined(__SSE2__) || defined(__MMX__)
    h = (Histogram*) _mm_malloc( n * sizeof(Histogram), 16 );
    memset( h, 0, n * sizeof(Histogram) );
#else
    h = (Histogram*) calloc( n, sizeof(Histogram) );
#endif

    /* First row initialization */
    for ( j = 0; j < n; ++j ) {
        HOP( h[j], 0, += r );
    }
    for ( i = 0; i < r; ++i ) {
        for ( j = 0; j < n; ++j ) {
            HOP( h[j], src[src_step_row*i+j*src_step_col], ++ );
        }
    }

    for ( j = 0; j < (pad_left ? r : 2*r); ++j ) {
        HOP( h[j], src[src_step_row*r+j*src_step_col], ++ );
    }

    /* First column initialization */
    memset( &H, 0, sizeof(Histogram) );
    if ( pad_left ) {
        HOP( H, 0, += (2*r+1) * r );
    }
    for ( j = 0; j < (pad_left ? r : 2*r); ++j ) {
        histogram_add( &h[j], &H );
    }

    i = 0;
    while (1) {

        j = pad_left ? 0 : r;

        /* 
         * I use SSE2 to fetch 16 pixels and select their 4 high bits. This is
         * only advantageous if the horizontal dimension has unit stride.
         */
#ifdef __SSE2__
        if ( src_step_col == 1 && i+r < m ) {
            buffer = *(const __m128i*) ( (uintptr_t) (src + src_step_row*(i+r) + src_step_col*(j+r)) & ~0xF );
            shifted = _mm_and_si128( mask, _mm_srli_epi64(buffer, 4) );
        }
#endif

        /* Right */
        for ( ; j < (pad_right ? n : n-r); ++j ) {
            if ( j+r < n ) {
                if ( i+r >= m ) {
                    HOP( h[j+r], 0, ++ );
                }
                else {
#ifdef __SSE2__
                    if ( src_step_col == 1 ) {
                        const unsigned char* address = src + src_step_row*(i+r) + src_step_col*(j+r);
                        int offset = (int) ( (uintptr_t) address & 0xF );
                        if ( offset == 0 ) {
                            buffer = *(const __m128i*) address;
                            shifted = _mm_and_si128( mask, _mm_srli_epi64(buffer, 4) );
                        }
                        ++h[j+r].coarse[ *((unsigned char*) &shifted + offset) ];
                        ++h[j+r].fine[ *((unsigned char*) &buffer + offset) ];
                    }
                    else {
#endif
                        /* 
                         * The above code using SSE2 does exactly what the
                         * following line does but for 16 bits at a time.
                         */
                        HOP( h[j+r], src[src_step_row*(i+r)+src_step_col*(j+r)], ++ );
#ifdef __SSE2__
                    }
#endif
                }
                histogram_add( &h[j+r], &H );
            }
            else {
                assert(pad_right);
                HOP( H, 0, += 2*r+1 );
            }

            dst[dst_step_row*i+dst_step_col*j] = histogram_median( &H, r );

            if ( j >= r ) {
                histogram_sub( &h[j-r], &H );
                if ( i < r ) {
                    HOP( h[j-r], 0, -- );
                }
                else {
                    HOP( h[j-r], src[src_step_row*(i-r)+src_step_col*(j-r)], -- );
                }
            }
            else {
                assert(pad_left);
                HOP( H, 0, -= 2*r+1 );
            }
        }

        /* Go down */
        for ( j = (pad_right ? n-r : n-2*r); j < n; ++j ) {
            if ( i < r ) {
                HOP( h[j], 0, -- );
                HOP( H, 0, -- );
            }
            else {
                HOP( h[j], src[src_step_row*(i-r)+src_step_col*j], -- );
                HOP( H, src[src_step_row*(i-r)+src_step_col*j], -- );
            }
        }
        ++i;
        if ( i >= m ) {
            break;
        }
        for ( j = n-1; j >= (pad_right ? n-r : n-2*r); --j ) {
            if ( i+r >= m ) {
                HOP( h[j], 0, ++ );
                HOP( H, 0, ++ );
            }
            else {
                HOP( h[j], src[src_step_row*(i+r)+src_step_col*j], ++ );
                HOP( H, src[src_step_row*(i+r)+src_step_col*j], ++ );
            }
        }

        j = pad_right ? n-1 : n-1-r;

#ifdef __SSE2__
        if ( src_step_col == 1 && i+r < m ) {
            buffer = *(const __m128i*) ( (uintptr_t) (src + src_step_row*(i+r) + src_step_col*(j-r)) & ~0xF );
            shifted = _mm_and_si128( mask, _mm_srli_epi64(buffer, 4) );
        }
#endif

        /* Left */
        for ( ; j >= (pad_left ? 0 : r) ; --j ) {
            if ( j >= r ) {
                if ( i+r >= m ) {
                    HOP( h[j-r], 0, ++ );
                }
                else {
#ifdef __SSE2__
                    if ( src_step_col == 1 ) {
                        const unsigned char* address = src + src_step_row*(i+r) + src_step_col*(j-r);
                        int offset = (int) ( (uintptr_t) address & 0xF );
                        ++h[j-r].coarse[ *((unsigned char*) &shifted + offset) ];
                        ++h[j-r].fine[ *((unsigned char*) &buffer + offset) ];
                        if ( offset == 0 ) {
                            buffer = *(const __m128i*) ((uintptr_t) address - 0x10);
                            shifted = _mm_and_si128( mask, _mm_srli_epi64(buffer, 4) );
                        }
                    }
                    else {
#endif
                        HOP( h[j-r], src[src_step_row*(i+r)+src_step_col*(j-r)], ++ );
#ifdef __SSE2__
                    }
#endif
                }
                histogram_add( &h[j-r], &H );
            }
            else {
                assert(pad_left);
                HOP( H, 0, += 2*r+1 );
            }

            dst[dst_step_row*i+dst_step_col*j] = histogram_median( &H, r );

            if ( j+r < n ) {
                histogram_sub( &h[j+r], &H );
                if ( i < r ) {
                    HOP( h[j+r], 0, -- );
                }
                else {
                    HOP( h[j+r], src[src_step_row*(i-r)+src_step_col*(j+r)], -- );
                }
            }
            else {
                assert(pad_right);
                HOP( H, 0, -= 2*r+1 );
            }
        }

        /* Go down */
        for ( j = (pad_left ? r-1 : 2*r-1); j >= 0; --j ) {
            if ( i < r ) {
                HOP( h[j], 0, -- );
                HOP( H, 0, -- );
            }
            else {
                HOP( h[j], src[src_step_row*(i-r)+src_step_col*j], -- );
                HOP( H, src[src_step_row*(i-r)+src_step_col*j], -- );
            }
        }
        ++i;
        if ( i >= m ) {
            break;
        }
        for ( j = 0; j < (pad_left ? r : 2*r); ++j ) {
            if ( i+r >= m ) {
                HOP( h[j], 0, ++ );
                HOP( H, 0, ++ );
            }
            else {
                HOP( h[j], src[src_step_row*(i+r)+src_step_col*j], ++ );
                HOP( H, src[src_step_row*(i+r)+src_step_col*j], ++ );
            }
        }
    }

#if defined(__SSE2__) || defined(__MMX__)
    _mm_empty();
    _mm_free(h);
#else
    free(h);
#endif
}

/**
 * \brief Constant-time median filtering
 *
 * This function does a median filtering of an 8-bit image. The source image is
 * processed as if it was padded with zeros. The median kernel is square with
 * odd dimensions. Images of arbitrary size may be processed.
 *
 * To process multi-channel images, you must call this function multiple times,
 * changing the source and destination adresses and steps such that each channel
 * is processed as an independent single-channel image.
 *
 * Processing images of arbitrary bit depth is not supported.
 *
 * The computing time is O(1) per pixel, independent of the radius of the
 * filter. The algorithm's initialization is O(r*width), but it is negligible.
 * Memory usage is simple: it will be as big as the cache size, or smaller if
 * the image is small. For efficiency, the histograms' bins are 16-bit wide.
 * This may become too small and lead to overflow as \a r increases.
 *
 * \param src           Source image data.
 * \param dst           Destination image data. Must be preallocated.
 * \param width         Image width, in pixels.
 * \param height        Image height, in pixels.
 * \param src_step_row  Distance between adjacent pixels on the same column in
 *                      the source image, in bytes.
 * \param src_step_col  Distance between adjacent pixels on the same row in
 *                      the source image, in bytes.
 * \param dst_step_row  Distance between adjacent pixels on the same column in
 *                      the destination image, in bytes.
 * \param dst_step_col  Distance between adjacent pixels on the same column in
 *                      the destination image, in bytes.
 * \param r             Median filter radius. The kernel will be a 2*r+1 by
 *                      2*r+1 square.
 */
void ctmf( const unsigned char* const src, unsigned char* const dst, const int width, const int height,
        const int src_step_row, const int src_step_col, const int dst_step_row, const int dst_step_col,
        const int r )
{
    /*
     * Processing the image in vertical stripes is an optimization made
     * necessary by the limited size of the CPU cache. Each histogram is 544
     * bytes big and therefore I can fit a limited number of them in the cache.
     * That number may sometimes be smaller than the image width, which would be
     * the number of histograms I would need without stripes.
     *
     * I need to keep histograms in the cache so that they are available
     * quickly when processing a new row. Each row needs access to the previous
     * row's histograms. If there are too many histograms to fit in the cache,
     * thrashing to RAM happens.
     *
     * To solve this problem, I figure out the maximum number of histograms
     * that can fit in cache. From this is determined the number of stripes in
     * an image. The formulas below make the stripes all the same size and use
     * as few stripes as possible.
     *
     * Note that each stripe causes an overlap on the neighboring stripes, as
     * when mowing the lawn. That overlap is proportional to r. When the overlap
     * is a significant size in comparison with the stripe size, then we are not
     * O(1) anymore, but O(r). In fact, we have been O(r) all along, but the
     * initialization term was neglected, as it has been (and rightly so) in B.
     * Weiss, "Fast Median and Bilateral Filtering", SIGGRAPH, 2006. Processing
     * by stripes only makes that initialization term bigger.
     *
     * Also, note that the leftmost and rightmost stripes don't need overlap.
     * A flag is passed to ctmf_helper() so that it treats these cases as if the
     * image was zero-padded.
     */
    const int CACHE_SIZE  = 256 * 1024;
    const int STRIPES     = (int)ceil( (double) (width - 2 * r) / (CACHE_SIZE / sizeof(Histogram) - 2 * r) );


    const int STRIPE_SIZE = (int)ceil( (double) ( width + STRIPES * 2 * r - 2 * r ) / STRIPES );

    int i;

    for ( i = 0; i < width; i += STRIPE_SIZE - 2 * r )
	{
        int stripe = STRIPE_SIZE;
        /* Make sure that the filter kernel fits into one stripe. */
        if ( i + STRIPE_SIZE - 2 * r >= width || width - (i + STRIPE_SIZE - 2 * r) < 2 * r + 1 )
		{
            stripe = width - i;
        }

        ctmf_helper( src + i*src_step_col, dst + i * dst_step_col, stripe, height,
                src_step_row, src_step_col, dst_step_row, dst_step_col,
                r, i == 0, stripe == width - i );

        if ( stripe == width - i ) {
            break;
        }
    }
}
