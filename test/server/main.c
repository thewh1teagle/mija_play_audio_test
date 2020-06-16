#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>



#define _LARGEFILE_SOURCE 1


int main(int argc,char **argv) {
  int fd;
  int status;
  FILE *__stream;
  unsigned char *file_content_to_send;
  size_t read_count;
  struct stat stat_buf;
  char file_name [256];
  
  memset(file_name,0,0x100);
  shbf_rcv_global_init();
  fd = shbf_rcv_create("/run/audio_play");
  status = shbf_rcv_start();
  if (status < 0) {
    perror("shbf_rcv_start");
  }
  else {
    shbf_rcv_fd(fd);
    if (argc == 2) {
      strcpy(file_name,argv[1]);
      puts(file_name);
      __stream = fopen(file_name,"rb");
      if (__stream == (FILE *)0x0) {
        printf("Open %s failed: %m\n",file_name);
      }
      else {
        printf("playing audio file: [%s]\n",file_name);
        stat(file_name,&stat_buf);
        printf("filesize is %d\n",stat_buf.st_size);
        file_content_to_send = (unsigned char *)malloc((size_t)stat_buf.st_size);
        if (file_content_to_send == (unsigned char *)0x0) {
          printf("[audio_player] malloc error");
        }
        else {
          read_count = fread(file_content_to_send,1,(size_t)stat_buf.st_size,__stream);
          printf("send len is %d\n",read_count);
          if (0 < (int)read_count) {
            shbf_set_size(fd,read_count);
            shbf_rcv_send_message(fd,file_content_to_send,read_count);
            puts("send over");
            free(file_content_to_send);
            fclose(__stream);
            return 0;
          }
        }
      }
    }
  }
  return 1;
}