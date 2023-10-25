#include <iostream>
#include <vector>
#include <mutex>
#include <pthread.h>
#include "SyncStream.h"

SyncStream source;
SyncStream sink;

void* thr_convert( void* o )
{
    printf( "Convert started\n" );

    while (!source.isEnd())
    {
        int b = source.read();
        if (b < 0) break;

        int fl = (b >> 6) & 3;
        b = b & ( (1 << 6)-1 );

        char buf[100];

        if (fl == 0)
            sprintf( buf, "%d\n", b );
        else if (fl == 1)
        {
            int bit = b >> 5;
            int8_t bb = (b & 0x3F) | (bit << 6) | (bit << 7);

            sprintf( buf, "%d\n", (int) bb );
        }
        else
        {
            char ch = b + 'a';

            sprintf( buf, "%c\n", ch );
        }

        sink.write( buf );
    }

    sink.signalEnd();

    printf( "Convert ended\n" );
}

void* thr_write( void* )
{
    printf( "Write started\n" );

    FILE *f = fopen( "res.txt", "wt" );

    while (!sink.isEnd())
    {
        int b = sink.read();
        if (b < 0)  break;

        fprintf( f, "%c", b );
    }

    fclose(f);

    printf( "Write ended\n" );
}

int main()
{
    printf( "Reading file\n" );
    FILE *f = fopen( "in.dat", "rb" );

    if (f == nullptr)
    {
        printf( "Failed to open in.dat\n" );
        return 1;
    }

    printf( "Creating threads\n" );
    pthread_t thr1, thr2;

    while (!feof(f))
    {
        char ch;

        if (fread( &ch, 1,1, f ) != 1)
            break;

        source.write(ch);
    }

    source.signalEnd();

    pthread_create( &thr1, nullptr, &thr_convert, nullptr );
    pthread_create( &thr2, nullptr, &thr_write, nullptr );

    //thr_convert( nullptr );
    //thr_write( nullptr );

    printf( "Waiting threads\n" );
    //pthread_join( thr1, nullptr );
    //pthread_join( thr2, nullptr );

    timespec t{1,0};
    nanosleep( &t, nullptr );

    printf( "End\n" );
    return 0;
}
