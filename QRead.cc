#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using std::cout; using std::endl;
using std::string; using std::copy;

const char* map_file(const char* fname, size_t& length);

int main(int argc, char* argv[])
{
    size_t length;
    long int count = 0;
    const char* f = map_file(argv[1], length);
    const char* l = f + length;

    uintmax_t m_numLines = 0;
    while (f && f!=l)
        if ((f = static_cast<const char*>(memchr(f, '\n', l-f))))
        {
            cout << f << endl; 
            m_numLines++; f++;
        }
    std::cout << "m_numLines = " << m_numLines << "\n";

    std::cout << "Tapi calls: " << count << endl;
}

void handle_error(const char* msg) 
{
    perror(msg); 
    exit(255);
}

const char* map_file(const char* fname, size_t& length)
{
    int fd = open(fname, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    // TODO close fd at some point in time, call munmap(...)
    return addr;
}
