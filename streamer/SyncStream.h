#ifndef SYNCSTREAM_H
#define SYNCSTREAM_H

#include <iostream>
#include <vector>
#include <mutex>

using namespace std;

class SyncStream
{
private:
    mutex crit;
    bool end = false;
    vector<uint8_t> buf;
public:
    SyncStream();

    void write( uint8_t v );
    void write( char *s );
    void signalEnd();

    bool isEnd();

    int read();
};

#endif // SYNCSTREAM_H
