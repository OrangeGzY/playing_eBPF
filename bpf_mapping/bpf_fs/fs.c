#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include "../../bpf.c"

static const char * file_path = "/sys/fs/bpf/my_array";

int get_fs_file(){
    int fd, key, value, res;

    fd = bpf_obj_get(file_path);
    if(fd<0){
	printf("failed to fetch the map: %d(%s)\n",fd, strerror(errno));
	return -1;
    }
    
    key = 1;
    res = bpf_map_lookup_elem(fd, &key, &value);
    if(res<0){
	printf("failed to lookup the key: %d(%s)\n",fd, strerror(errno));
	return -1;
    }
    printf("value read from the map: '%d'\n",value);
    return 0;
}

int main(){
   
    get_fs_file();
    getchar();
    int key,value, fd, added , pinned;
    fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100,0);
    if(fd<0){
	printf("create failed : %d(%s)\n",fd,strerror(errno));
        return -1;
    }
    key = 1;
    value = 1234;

    added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
    if(added<0){
	printf("update failde : %d(%s)\n",added,strerror(errno));
        return -1;
    }

    pinned = bpf_obj_pin(fd, file_path);
    if(pinned<0){
	printf("failed to pin map to fs: %d(%s)\n",pinned, strerror(errno));
	return -1;
    }
    

    


    return 0;
}
