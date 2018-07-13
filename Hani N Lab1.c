
/*
* @author: Hani Nguyen
* @course: CSE 3320 - 001
* @lab no: 1
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <limits.h>

#define FILE_NUMBER 1024
#define FILE_NAME 2048


//Function for string comparison. This helps in partial name completion for directories.
void string_comparison(int index_arr[], char user_s[256], char s[FILE_NUMBER][FILE_NAME], int size, int *index_size) {

    int file_numbers[1024];
    int file_size = 0;

    // int index_arr[size];

    for(int i = 0; i < size; i++) {
        if(strcmp(user_s, s[i]) == 0) {
            index_arr[*index_size] = i;
            (*index_size)++;
            return;
        }
    }

    if(((int)(*user_s) - 48) < size) {

        index_arr[*index_size] = (int)(*user_s) - 48;
        (*index_size)++;
        return;
    }


    for(int i = 0; i < size; i++) {
        char temp[strlen(user_s)+1];
        strncpy(temp, s[i], strlen(user_s));
        temp[strlen(user_s)] = '\0';
        if(strcmp(temp, user_s) == 0) {
            index_arr[*index_size] = i;
            (*index_size)++;
        }
    }

}

//simple bubble sort method.
void sort_array(char s[FILE_NUMBER][FILE_NAME], int *index_inc) {

    for(int i = 0; i < (*index_inc)-1; i++) {
        for(int j = i+1; j < (*index_inc); j++) {
            if(strcmp(s[i], s[j]) > 0) {
                char temp[FILE_NAME];
                strcpy(temp, s[i]);
                strcpy(s[i], s[j]);
                strcpy(s[j], temp);
            }
        }
    }
}

//clear the user input.
void clearInputBuffer(char c) {

    do {
        c = getchar();

    } while(c != '\n' && c != EOF);
}

void read_dir(char s[FILE_NUMBER][FILE_NAME], int *index_inc, DIR *d, struct dirent *de, int flag) {

    int d_type = (flag == 0) ? DT_DIR:DT_REG;

    while((de = readdir(d))) {
        if((de->d_type) & d_type) {
            strcpy(s[*index_inc], de->d_name);
            (*index_inc)++;
        }
    }
}

int main(void) {
    
    
    pid_t child; /*Getting the process ID*/
    DIR * d;
    struct dirent * de;
    int i, c, k;
    char s[256], cmd[256];
    struct stat buffer;
    time_t t;

    int f_view = 8;
    int f_start_view = 0;
    int d_view = 8;
    int d_start_view = 0;
    char file_s[FILE_NUMBER][FILE_NAME];
    char directory_s[FILE_NUMBER][FILE_NAME];
    int farray_size = 0;
    int darray_size = 0;
    d = opendir(".");
    read_dir(file_s, &farray_size, d, de, 1);
    d = opendir(".");
    read_dir(directory_s, &darray_size, d, de, 0);

    while(1) {
        system("clear");
        t = time(NULL); /*Getting the time from the system*/
        printf("\n-----------------------------------------------------\n");
        getcwd(s, 200);
        printf("Current Working Dir: %s \n", s);
        printf("It is now: %s\n", ctime(&t));

        //Reading the files in the current directory.
        c = 0;
        printf("\t\tFILES ( %d / %d )\n\n", f_view/8, (farray_size/8 > 0) ? farray_size/8:1);
        for(int i = f_start_view; i < farray_size; i++) {

            if(i <= f_view) {
                stat(file_s[i], &buffer);
                printf("%d | ", i);
                printf( "%s", (S_ISDIR(buffer.st_mode)) ? "d" : "-");
                printf( "%s", (buffer.st_mode & S_IRUSR) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWUSR) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXUSR) ? "x" : "-");
                printf( "%s", (buffer.st_mode & S_IRGRP) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWGRP) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXGRP) ? "x" : "-");
                printf( "%s", (buffer.st_mode & S_IROTH) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWOTH) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXOTH) ? "x" : "-");
                printf(" | %s \n", file_s[i]);
            }
        }

        printf("\n");
        //Reading the directories in the current directory.
        printf("\t\tDIRECTORIES ( %d / %d ) \n\n", d_view/8, (darray_size/8 > 0) ? darray_size/8:1);
        for(int i = d_start_view; i < darray_size; i++) {

            if(i <= d_view) {
                stat(directory_s[i], &buffer);
                printf("%d | ", i);
                printf( "%s", (S_ISDIR(buffer.st_mode)) ? "d" : "-");
                printf( "%s", (buffer.st_mode & S_IRUSR) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWUSR) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXUSR) ? "x" : "-");
                printf( "%s", (buffer.st_mode & S_IRGRP) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWGRP) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXGRP) ? "x" : "-");
                printf( "%s", (buffer.st_mode & S_IROTH) ? "r" : "-");
                printf( "%s", (buffer.st_mode & S_IWOTH) ? "w" : "-");
                printf( "%s", (buffer.st_mode & S_IXOTH) ? "x" : "-");
                // printf("\n\n");
                printf(" | %s \n", directory_s[i]);
            }
        }

        printf("\n");

        printf("-----------------------------------------------------\n");

        printf("E. Edit \t\t C. Change\n");
        printf("M. View More \t\t L. View Less\n");
        printf("R. Run  \t\t S. Sort\n");
        printf("Q. Quit\n");
        printf("-----------------------------------------------------\n");
        
        printf("\n");
        printf("~$: ");

        c = getchar(); getchar();

        c = tolower(c);

        int index_arr[darray_size];
        switch(c) {
            case 'q': exit(0);
            case 'e': printf("Edit what?: ");
                scanf("%s", s);
                strcpy(cmd, "vim ");
                strcat(cmd, s);
                system(cmd);
                break;
            case 'r':
                printf("You can add the parameters to your command by adding a space, for example \"ls -l\" would work.\n");
                printf("Run what?: ");
                scanf("%[^\n]s", cmd);
                system(cmd);
                printf("\nPress any key to continue\n");
                getchar();
                break;
            case 'c':
                printf("You can either enter full directory name, partial directory name or the directory number.\n");
                printf("Change to?: ");
                scanf("%s", cmd);
                int index_size = 0;
                string_comparison(index_arr, cmd, directory_s, darray_size, &index_size);
                if(index_size > 1) {
                    for(int i = 0; i < index_size; i++) {
                        printf("\n %d. %s", i, directory_s[index_arr[i]]); 
                    }

                    int direct;
                    printf("\n\nEnter the directory number: ");
                    scanf("%d", &direct);

                    if(direct < index_size) {
                        chdir(directory_s[index_arr[direct]]);
                    }
                } else {
                    chdir(directory_s[index_arr[0]]);
                }

                d = opendir(".");

                farray_size = 0;
                darray_size = 0;

                read_dir(file_s, &farray_size, d, de, 1);
                d = opendir(".");
                read_dir(directory_s, &darray_size, d, de, 0);
                break;
            case 'm':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input = getchar();
                input = tolower(input);
                switch(input) {
                    case 'd':
                        d_view = (d_view+8 < darray_size) ? d_view+8 : darray_size;
                        d_start_view = (d_start_view+8 < darray_size) ? d_start_view+8 : d_start_view;
                        break;
                    case 'f':
                        f_view = (f_view+8 < farray_size) ? f_view+8 : farray_size;
                        f_start_view = (f_start_view+8 < farray_size) ? f_start_view+8 : f_start_view;
                        break;
                }
                break;
            case 'l':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input2 = getchar();
                input2 = tolower(input2);
                switch(input2) {
                    case 'd':
                        d_view = (d_view-8 > 8) ? d_view-8 : 8;
                        d_start_view = (d_start_view-8 > 0) ? d_start_view-8 : 0;
                        break;
                    case 'f':
                        f_view = (f_view-8 > 8) ? d_view+8 : 8;
                        f_start_view = (f_start_view-8 > 0) ? f_start_view-8 : 0;
                        break;
                }
                break;

            case 's':
                printf("-----------------------------------------------------\n");
                printf("D. Directory \t F. Files\n");
                printf("-----------------------------------------------------\n");
                printf("~$: ");
                int input3 = getchar();
                input3 = tolower(input3);
                switch(input3) {
                    case 'd':
                        sort_array(directory_s, &darray_size);
                        break;
                    case 'f':
                        sort_array(file_s, &farray_size);
                        break;
                }
                break;

        }
        //clearing input buffer:
        clearInputBuffer(c);
    }
}
