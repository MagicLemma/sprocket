#include <string.h>
#include "utf8-utils.h"

namespace ftgl {

uint32_t
utf8_to_utf32( const char * character )
{
    uint32_t result = -1;

    if( !character )
    {
        return result;
    }

    if( ( character[0] & 0x80 ) == 0x0 )
    {
        result = character[0];
    }

    if( ( character[0] & 0xC0 ) == 0xC0 )
    {
        result = ( ( character[0] & 0x3F ) << 6 ) | ( character[1] & 0x3F );
    }

    if( ( character[0] & 0xE0 ) == 0xE0 )
    {
        result = ( ( character[0] & 0x1F ) << ( 6 + 6 ) ) | ( ( character[1] & 0x3F ) << 6 ) | ( character[2] & 0x3F );
    }

    if( ( character[0] & 0xF0 ) == 0xF0 )
    {
        result = ( ( character[0] & 0x0F ) << ( 6 + 6 + 6 ) ) | ( ( character[1] & 0x3F ) << ( 6 + 6 ) ) | ( ( character[2] & 0x3F ) << 6 ) | ( character[3] & 0x3F );
    }

    if( ( character[0] & 0xF8 ) == 0xF8 )
    {
        result = ( ( character[0] & 0x07 ) << ( 6 + 6 + 6 + 6 ) ) | ( ( character[1] & 0x3F ) << ( 6 + 6 + 6 ) ) | ( ( character[2] & 0x3F ) << ( 6 + 6 ) ) | ( ( character[3] & 0x3F ) << 6 ) | ( character[4] & 0x3F );
    }

    return result;
}

}
