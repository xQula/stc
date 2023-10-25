#include "SyncStream.h"

using namespace std;

SyncStream::SyncStream()
{
}

void SyncStream::write( uint8_t v )
{
    crit.lock();

    buf.push_back(v);

    crit.unlock();
}
void SyncStream::write( char *s )
{
    crit.lock();

    for( int i = 0; s[i] != 0; i++ )
        buf.push_back( (uint8_t) s[i] );

    crit.unlock();

}
void SyncStream::signalEnd()
{
    end = true;
}

bool SyncStream::isEnd()
{
    return end & (buf.size() == 0);
}

int SyncStream::read()
{
    while (true)
    {
        int r = -1;

        crit.lock();
        if (buf.size() > 0)
        {
            r = buf[0];
            buf.erase( buf.begin() );
        }
        crit.unlock();

        if (r >= 0)
            return r;

        if (end & (buf.size() == 0))
            break;

        const timespec t{ 0, 1000*1000 };
        nanosleep( &t, nullptr );
    }

    return -1;
}
