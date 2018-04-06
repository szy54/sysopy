#define _XOPEN_SOURCE 700
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include <ftw.h>

//All the code belongs to github.com/osdnk on his GPL license

//#define buff_size PATH_MAX;

const char format[] = "%Y-%m-%d %H:%M:%S";
char buffer[PATH_MAX];

void printDetails(char *path, struct dirent *rdir, struct stat *file_stat, char *buffer){
    printf("%s\t", realpath(path, buffer));
    printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");
    printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");
    printf(" %ld\t", file_stat->st_size);
    strftime(buffer, PATH_MAX, format, localtime(&file_stat->st_mtime));
    printf(" %s\t", buffer);
    printf("\n");
}


void fileInfoGrabber (char * path, char *op, time_t date){
    if(path==NULL)
        return;
    DIR *dir = opendir(path);
    if(dir==NULL)
        return;
    struct dirent *rdir = readdir(dir);
    struct stat file_stat;

    char new_path[PATH_MAX];

    while(rdir!=NULL){
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, rdir->d_name);

        stat(new_path, &file_stat);

        if(S_ISLNK(file_stat.st_mode)){
            rdir=readdir(dir);
            continue;
        }
        else if(strcmp(rdir->d_name, ".")==0 || strcmp(rdir->d_name, "..")==0){
            rdir=readdir(dir);
            continue;
        }
        else{
            if(S_ISREG(file_stat.st_mode)){
                if(strcmp(op, "=") ==0){
                    if(difftime(date, file_stat.st_mtime)==0)
                        printDetails(new_path, rdir, &file_stat, buffer);
                }
                else if(strcmp(op, "<")==0){
                    if(difftime(date, file_stat.st_mtime)>0)
                        printDetails(new_path, rdir, &file_stat, buffer);
                }        
                else if(strcmp(op, ">")==0){
                    if(difftime(date, file_stat.st_mtime)<0)
                        printDetails(new_path, rdir, &file_stat, buffer);
                }
            }
            if(S_ISDIR(file_stat.st_mode)){
                fileInfoGrabber(realpath(rdir->d_name, new_path), op, date);
            }
            rdir=readdir(dir);
        }
    }
    closedir(dir);
}

int main (int argc, char *argv[]){
    //argv[1] - sciezka
    //argv[2] - < > =
    //argv[3] - data
    if(argc<4){
        printf("too few arguments");
    }

    char *path=argv[1];
    char *sign=argv[2];
    char *usrDate=argv[3];

    printf("%s %s %s\n", path, sign, usrDate);
    struct tm *tm=malloc(sizeof(struct tm));
    strptime(usrDate, format, tm);
    strftime(buffer, PATH_MAX, format, tm);
    time_t date = mktime(tm);
    
    DIR *dir = opendir(realpath(path, NULL));
    if(dir ==NULL){
        printf("couldn't open the dir\n");
        return 1;
    }
    fileInfoGrabber(realpath(path, NULL), sign, date);

    closedir(dir);


    return 0;

}