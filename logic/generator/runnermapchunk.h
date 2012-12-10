#ifndef _RUNNER_MAP_CHUNK_INCLUDED_
#define _RUNNER_MAP_CHUNK_INCLUDED_

#include <vector>

class RunnerMapChunk
{
public:
    RunnerMapChunk();
    RunnerMapChunk(const RunnerMapChunk& c);

    void set(unsigned int i, unsigned char val);
    unsigned char get(unsigned int i);

private:

    int m_offset;
    std::vector<unsigned char> tile;

};

#endif // _RUNNER_MAP_CHUNK_INCLUDED_
