#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int Show(char ff[12])
{ 
  int fd,fr;
  char buf[1000]; 
  fd = open(ff , O_RDONLY);
  printf("file name : ");
  fr=read(fd, buf, 1000);
  printf("%s", buf);

 if (fd ==-1) {
    fprintf(stderr, "no filename present\n");
    exit(1);
   }
  
  
}


int Count(char ff[12]){
  int fd;
  fd = open(ff , O_RDONLY);
  if (fd ==-1)
  {
    fprintf(stderr, "no filename present\n");
    exit(1);
  }
  int length, i;
  int counter = 0;
  char buffer[fd];
   //int size = (int)(sizeof(buffer[fd])) ;
  length = read(fd, buffer,1000);  

  for (i = 0; i<length; i++) 
  { 
    
    if(buffer[i] =='\n')  counter++ ;
   
  }
   return counter;
}


int Erase(char ff[12]){
  int fd;
  fd = open(ff , O_RDONLY);
  if (fd ==-1)
  {
    fprintf(stderr, "no filename present\n");
    exit(1);
  }
  remove(ff);
}
int Inform(char ff[12]){
  int fd;
  fd = open(ff , O_RDONLY);
  if (fd ==-1)
  {
    fprintf(stderr, "no filename present\n");
    exit(1);
  }
  struct stat sfile; 
   if(stat(ff,&sfile)==-1){
     fprintf(stderr, "Error Occurred\n");
  }

 //Accessing data members of stat struct
 printf("\nFile name %s \n",ff);
 printf("\nFile size %ld \n",sfile.st_size);
 printf("\nTime of last access %ld \n",sfile.st_atime);
 printf("\nTime of last modification %ld \n",sfile.st_mtime);
 printf("\nFile link %u \n",(unsigned int)sfile.st_nlink);
 printf("\nFile user ID %d \n",sfile.st_uid);
  printf("\nFile i-node %lo \n",sfile.st_ino);

 close(fd);
}


int Append(char ff[12],char ff2[12]){
  int fd;
  int fd2;
  fd = open(ff , O_WRONLY| O_APPEND);
  fd2 = open(ff2 , O_RDONLY);
  if (fd ==-1)
  {
    fprintf(stderr, "no filename present\n");
    exit(1);
  }
  if (fd2 ==-1)
  {
    fprintf(stderr, "no filename present\n");
    exit(1);
  }
  int fr, fw, n, fr1;
  char buf[1000];
  char copy[1000];
  
   while((fr=read(fd2, buf, 1000)) != 0) {

        if (fr < 0) {
           fprintf(stderr, "Error reading source file\n");
           exit(1);
        }
        fr1=read(fd, copy, 1000);
        strcat(copy,buf);
        fw=0;
       
        do{
             n=write(fd, copy , fr);
         
            if (n < 0)
            {
               fprintf(stderr, "Error writing destination file\n");
               exit(1);
             }
         
          fw += n;
         

        } while (fw < fr);

      }

      close(fd); close(fd2);
  close(fd);
}


void  List(char dir[50], int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            List(entry->d_name,depth+4);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);

}

int main(int argc, char* argv[])
{
   int a;
   printf("what do you want to do: \n");
   printf("1. OPEN \n"); 
   printf("2. COUNT \n");
   printf("3. ERASE \n");
   printf(" 4. INFORM \n");      
   printf(" 5. APPEND \n");
   printf(" 6.LIST \n");
         
   scanf("%d",&a);
    char f[12];
    char f2[12];
    char dir[50];
    int i,uzunluk;
    char mainpath[50]="/home/minekaya/Desktop";
    char no[50]="0";
  switch(a){
      case 1:
          printf("give a file name:");
          scanf("%s", f);
          Show(f);
          break;
       case 2:
          printf("give a file name:");
          scanf("%s", f);
          printf("%d",Count(f));
          break;
       case 3:
          printf("give a file name:");
          scanf("%s", f);
          Erase(f);
          break;
       case 4:
          printf("give a file name:");
          scanf("%s", f);
          Inform(f);
          break;
       case 5:
          
          printf("give a file name 1:");
          scanf("%s", f);
          printf("give a file name 2:");
          scanf("%s", f2);
          Append(f,f2);
          break;
       case 6:
          printf("give directory for scan \n ( if you wanna know current directory write 0):");
          scanf("%s", dir);
          if(strcmp(dir,no) == 0) List(mainpath,0);
          List(dir,0);         
          printf("done.\n");
          break;
}
}