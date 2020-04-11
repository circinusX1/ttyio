
#ifndef TTY_SERIAL_H
#define TTY_SERIAL_H

#include <termios.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
class Tty
{
public:
    Tty(const char*, int, const char *);
    ~Tty();
    bool    iopen(int  mode=3);
    void    iclose();
    int     bread(uint8_t* buff, int len, int to=1024);
    int     bwrite(const uint8_t* buff, int len, int to=0);
    void    flush();
protected:
    int _com_setting(const char* cmd,int opt=-1);
    int _async_read(unsigned char *, int, int to=100, int sto=2);
    int _is_dcd_enabled();
    int _is_cts_enabled();
    int _is_dsr_enabled();
    void _enable_dtr();
    void _disable_dtr();
    void _enable_rts();
    void _disable_rts();

#if defined (__linux__) || defined(__FreeBSD__)
    struct termios _curTCSett;
    struct termios _oldTCSett;
    int            _port;
#else
    HANDLE         _port;
#endif
    int         _error;
    std::string _mode;
    std::string _dev_node;
    int         _bouts=0;
};


#define LOGE(X)  std::cerr<<X<<"\n";
#define LOGI(X)  std::cout<<X<<"\n";

inline size_t tick_count()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
        return 0;
    size_t retv = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return retv;
}

inline void msleep(time_t t)
{
    ::usleep(t*1000);
}

#endif // SERIAL_H
