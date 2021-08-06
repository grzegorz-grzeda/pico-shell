#ifndef PSH_RAMDISC_H__
#define PSH_RAMDISC_H__

#include "fs/psh_fs.h"
#include "utils/psh_errors.h"

typedef struct{
    psh_file *file;
    struct {
        char *ptr;
        int size;
        int _written_cnt;
        int _read_cnt;
    }buffer;
}psh_ramdisc;

int psh_init_ramdisc(char *data_buffer, int data_size, int max_file_size, psh_ramdisc* ramdisc_buffer, int ramdisc_size);
int psh_attach_ramdisc_to_file(psh_file *f);

#endif