#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int strncmp(char* s1, char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] > s2[i])
            return 1;
        else if (s2[i] > s1[i])
            return -1;
    }
    return 0;
}

int contains(char* text, char* pattern) {
    int length_text, length_pattern;
    length_text = strlen(text);
    length_pattern = strlen(pattern);
    for (int i = 0; i <= length_text - length_pattern; i++){
        if (strncmp(text+i, pattern, length_pattern) == 0) {
            return 1;
        }
    } 
    return 0;
}

void find(char *path, char *pattern)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (contains(path, pattern)) {
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || 0 == strcmp(de.name, ".")|| 0 == strcmp(de.name, "..") )
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      
      switch (st.type)
      {
      case T_FILE:
        if (contains(buf, pattern)) {
            printf("%s\n", buf);
        }
        break;
      case T_DIR:
        find(buf, pattern);
        break;
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char* argv[]){
    if (argc < 3) {
        printf("two few arguments!\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}