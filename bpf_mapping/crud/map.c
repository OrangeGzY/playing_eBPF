#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <errno.h>
#include <linux/bpf.h>
//#include "bpf_load.h"
//#include "bpf_util.h"
//#include "libbpf.h"

static int bpf_create_map(enum bpf_map_type map_type, int key_size, int value_size,
        int max_entries, int flag){

    union bpf_attr attr = {
        .map_type = map_type,
        .key_size = key_size,
        .value_size = value_size,
        .max_entries = max_entries,
	.map_flags = flag
    };
    return syscall(__NR_bpf, BPF_MAP_CREATE, &attr, sizeof(attr));

}

static int bpf_update_elem(int fd ,void *key, void *value,uint64_t flags){
    union bpf_attr attr = {
        .map_fd = fd,
        .key = (uint64_t)key,
        .value = (uint64_t)value,
        .flags = flags,
    };
    return syscall(__NR_bpf, BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));

}
static int bpf_lookup_elem(int fd,void *key, void *value){
    union bpf_attr attr = {
        .map_fd = fd,
        .key = (uint64_t)key,
        .value = (uint64_t)value,
    };
    return syscall(__NR_bpf, BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
}
static int bpf_delete_elem(int fd, void *key)
{
    union bpf_attr attr = {
	.map_fd = fd,
	.key = (uint64_t)key,
    };
    return syscall(__NR_bpf, BPF_MAP_DELETE_ELEM, &attr,sizeof(attr));
}
static int bpf_get_next_key(int fd, void *lookup_key, void *next_key){
    union bpf_attr attr = {
	.map_fd = fd,
	.key = (uint64_t)lookup_key,
	.next_key = (uint64_t)next_key,
    };
    return syscall(__NR_bpf, BPF_MAP_GET_NEXT_KEY, &attr, sizeof(attr));
}

int main()
{
    /* 首先我们创建一个hash类型的bpf map */
    int fd; 
    fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), sizeof(int), 100, BPF_F_NO_PREALLOC);
    if(fd == -1){perror("bpf map failed");exit(-1);}
    printf("success create map\n");
  


    
    /* 尝试对此hash类型的bpf map的key:value进行更新 */
    int key, value, result;
    key = 1;
    value = 1234;
    result = bpf_update_elem(fd,&key,&value,BPF_ANY);
      
    if(result==0){
	printf("bpf update success!\n");
    }
    else
	printf("failed to update map with new value: %d(%s)\n",result, strerror(errno));
    
    /* 尝试创建相同key的元素（应当失败）*/
    //int key,value,res;
    int res;
    key = 1,value = 5678;
    res = bpf_update_elem(fd, &key, &value, BPF_NOEXIST);
    if(!res){
	printf("Map updated with new elem\n");
    }
    else
	printf("failed to update map with new val : %d(%s)\n", res, strerror(errno));
    
   
    /* 接下来我们尝试读取内核中的value */
    /* 成功读取后的值存储在read_val中 */
    int read_val;
    res = bpf_lookup_elem(fd, &key, &read_val);
    if(!res)
	printf("values from the map: '%d'\n",read_val);
    else
	printf("failed to lookup map: %d(%s)\n",res, strerror(errno));

    /* 删除对应的bpf key value */
    key = 1;
    res = bpf_delete_elem(fd, &key);
    if(!res)
	printf("delete success\n");
    else
	printf("failed to delete map: %d(%s)\n",res, strerror(errno));



    /* 添加多个新map */
    int new_key, new_value, it;
    for(it = 2; it<6; it++){
	new_key = it;
	new_value = 1234+it;
	bpf_update_elem(fd, &new_key, &new_value, BPF_NOEXIST);
    }

    /* 我们可以通过查找一个不存在的key来遍历整个map中的key */
    int next_key, lookup_key;
    lookup_key = -1;
    while(bpf_get_next_key(fd, &lookup_key, &next_key)==0){
	printf("next key in map is: '%d'\n",next_key);
	
        /* 查找时删除 */
	if(next_key==2){
	    printf("Delete key '2'\n");
	    bpf_delete_elem(fd, &next_key);
	}
	lookup_key = next_key;
    }



    return 0;
    



}
