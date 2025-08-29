#include <stddef.h>
#include <unistd.h>



// 协议头
typedef struct {
    unsigned char header[4];                // 协议头，4字节，例如 04 12 06 0A
    unsigned char version[1];               // 版本，1字节，00
    unsigned char partition[1];             // 分区
    unsigned char partition_length[2];      // 分区长度
    unsigned char partition_index[2];       // 分区索引
    unsigned char pid[2];                   // PID
    unsigned char pile[2];                  // 桩点类型
    unsigned char session_id[8];            // SESSION_id
    unsigned char total_length[4];          // 总长度
    unsigned char data_length[4];           // 数据长度
}ZMobADF_HEADER;


#define PARTITION 0x01
#define NOPARTITION 0x00
#define VERSION 0x00


extern ZMobADF_HEADER soc;

void initZMobADF(ZMobADF_HEADER *soc, size_t data_len);
void freeZMobADF(ZMobADF_HEADER *soc);

ZMobADF_HEADER initZMobADF_HEADER(int partation_TAG, int partation_length, int partation_index, int pid, int pile, int pile_type);

void setLength(size_t value, unsigned char* param);

void socketZMOBLOG(unsigned char* msg, ssize_t size);

// 小端序转换成大端序
void toBigEndian(void* dest, void* src, ssize_t size) ;