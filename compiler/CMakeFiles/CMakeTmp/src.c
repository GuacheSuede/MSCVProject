
                  #undef inline
#include <sys/types.h>
#include <sys/socket.h>
                  extern int 
                  recv(int, void *, unsigned int, unsigned int);
                  int main(void) {
                    int s=0;
                    void * buf=0;
                    unsigned int len=0;
                    unsigned int flags=0;
                    int res = recv(s, buf, len, flags);
                    (void) res;
                    return 0;
                  }
