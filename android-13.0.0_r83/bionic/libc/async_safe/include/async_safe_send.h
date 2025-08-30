#include <stddef.h>
#include <unistd.h>

// 协议头
typedef struct {
  unsigned char header[4];            // 协议头，4字节，例如 04 12 06 0A
  unsigned char version[1];           // 版本，1字节，00
  unsigned char partition[1];         // 分区
  unsigned char partition_length[2];  // 分区长度
  unsigned char partition_index[2];   // 分区索引
  unsigned char timestamp[8];         // 时间戳
  unsigned char pid[2];               // PID
  unsigned char pile[2];              // 桩点类型
  unsigned char session_id[8];        // SESSION_id
  unsigned char total_length[4];      // 总长度
  unsigned char data_length[4];       // 数据长度
} ZMobADF_HEADER;

#define PARTITION 0x01
#define NOPARTITION 0x00
#define VERSION 0x00

// 常量
#define ZMobADF_PILE_TYPE_FORK 0x01         // fork桩点
#define ZMobADF_PILE_TYPE_EXECVE 0x02       // execve桩点
#define ZMobADF_PILE_TYPE_EXIT 0x03         // exit桩点
#define ZMobADF_PILE_TYPE_OPEN 0x04         // open桩点
#define ZMobADF_PILE_TYPE_OPENAT 0x05       // openat桩点
#define ZMobADF_PILE_TYPE_CREAT 0x06        // creat桩点
#define ZMobADF_PILE_TYPE_UNLINK 0x07       // unlink桩点
#define ZMobADF_PILE_TYPE_UNLINKAT 0x08     // unlinkat桩点
#define ZMobADF_PILE_TYPE_RENAME 0x09       // rename桩点
#define ZMobADF_PILE_TYPE_RENAMEAT 0x0A     // renameat桩点
#define ZMobADF_PILE_TYPE_READ 0x0B         // read桩点
#define ZMobADF_PILE_TYPE_WRITE 0x0C        // write桩点
#define ZMobADF_PILE_TYPE_PREAD64 0x0D      // pread64桩点
#define ZMobADF_PILE_TYPE_PWRITE64 0x0E     // pwrite64桩点
#define ZMobADF_PILE_TYPE_CLOSE 0x0F        // close桩点
#define ZMobADF_PILE_TYPE_SOCKET 0x10       // socket桩点
#define ZMobADF_PILE_TYPE_CONNECT 0x11      // connect桩点
#define ZMobADF_PILE_TYPE_ACCEPT 0x12       // accept桩点
#define ZMobADF_PILE_TYPE_SENDTO 0x13       // sendto桩点
#define ZMobADF_PILE_TYPE_RECVFROM 0x14     // recvfrom桩点
#define ZMobADF_PILE_TYPE_SETSOCKOPT 0x15   // setsockopt桩点
#define ZMobADF_PILE_TYPE_GETSOCKOPT 0x16   // getsockopt桩点
#define ZMobADF_PILE_TYPE_BIND 0x17         // bind桩点
#define ZMobADF_PILE_TYPE_LISTEN 0x18       // listen桩点
#define ZMobADF_PILE_TYPE_SHUTDOWN 0x19     // shutdown桩点
#define ZMobADF_PILE_TYPE_FCNTL 0x1A        // fcntl桩点
#define ZMobADF_PILE_TYPE_IOCTL 0x1B        // ioctl桩点
#define ZMobADF_PILE_TYPE_GETSOCKNAME 0x1C  // getsockname桩点
#define ZMobADF_PILE_TYPE_GETPEERNAME 0x1D  // getpeername桩点
#define ZMobADF_PILE_TYPE_SOCKETPAIR 0x1E   // socketpair桩点
#define ZMobADF_PILE_TYPE_SENDMSG 0x1F      // sendmsg桩点
#define ZMobADF_PILE_TYPE_RECVMSG 0x20      // recvmsg桩点
#define ZMobADF_PILE_TYPE_CLONE 0x21        // clone桩点
#define ZMobADF_PILE_TYPE_VFORK 0x22        // vfork桩点
#define ZMobADF_PILE_TYPE_WAIT4 0x23        // wait4桩点
#define ZMobADF_PILE_TYPE_WAITPID 0x24      // waitpid桩点
#define ZMobADF_PILE_TYPE_KILL 0x25         // kill桩点
#define ZMobADF_PILE_TYPE_TKILL 0x26

extern ZMobADF_HEADER soc;

void initZMobADF(ZMobADF_HEADER* soc, size_t data_len);
void freeZMobADF(ZMobADF_HEADER* soc);

ZMobADF_HEADER initZMobADF_HEADER(int partition_TAG, int partition_length, int partition_index,
                                  int pid, int pile, int pile_type);

void setLength(size_t value, unsigned char* param);

void socketZMOBLOG(unsigned char* msg, ssize_t size);

// 小端序转换成大端序
void toBigEndian(void* dest, void* src, ssize_t size);