/*
	@author: Hani Nguyen
	@course: CSE 3320-001
	@lab no: 2 (PART A)
*/


 /*
    **********NOTE TO THE GRADER************
    1) How to run the program
     type: 
        $ g++ <filename>.cpp
        $ ./a.out 1 1 1
         (IMPORTANT: It is important to have the 4th parameter as 1)
    
    2) Input from the user: 
        a) Enter the number of processes.
    SIDE NOTE: My program will create 'n + 1' number of files depending on the 
    	number of processes user input. The files names would be 
    	'ag_axg9593_lab_<PID of the process>' and 'ag_axg9593_lab_mergedfile' 
    *****************************************
 */


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

#define convertToString(x) #x

using namespace std;


//defining all the global variables
char temp_string_buf1[10];
char temp_string_buf2[10];
int thread_count = 0;
int total_lines =  8762;
int sub_section = 0;
int start_value = 0;
int extra_padding = 0;
vector <int> pid_v;

char program_name[20];

//Helper function for reading the values from the CSV file.
const char* gf(char* line, int num)
{
    const char* token;
    for (token = strtok(line, ",");
        token && *token;
        token = strtok(NULL, ",\n"))
    {
        if (!--num)
            return token;
    }
    return NULL;
}
  


//creating parallel processes
void create_process(){
    for (int i=0; i < thread_count; i++){
 
        pid_t parent = getpid();
        pid_t pid = fork();
 
        if (pid == -1)
        {
            // error, failed to fork()
        }
        else if (pid > 0)
        {
            int status;
            //storing all the pid values for later.
            pid_v.push_back(pid);
            waitpid(pid, &status, 0);
        }
        else
        {
        	if (i == (thread_count -1)){
        		sub_section = sub_section + extra_padding;
        	}

        	vector<char *> commandVector;

			// do a push_back for the command, then each of the arguments
			commandVector.push_back(program_name);
			sprintf(temp_string_buf1,"%d",start_value);
			commandVector.push_back(temp_string_buf1);
			sprintf(temp_string_buf2,"%d",sub_section);
			commandVector.push_back(temp_string_buf2);
			commandVector.push_back(convertToString(3));

			// push NULL to the end of the vector (execvp expects NULL as last element)
			commandVector.push_back(NULL);

			// pass the vector's internal array to execvp
			char **command = &commandVector[0];

			int status = execvp(command[0], command);

			printf("%d\n", status);
			exit(0);
        }
       
        start_value = start_value + sub_section;
    }
}

int main(int argc, char* argv[]) {

	int start_index, number_of_lines, fork_flag;

	strcpy(program_name,argv[0]);

	//checking for the arguements
	if(argc < 3) {

		cout << "Usage: " << argv[0] << " <start index> <number of lines>" << endl;
		exit(0);
	} 

	//checking if the caller is the parent or not. THAT IS WHY THE 1 in the start is important.
	if (atoi(argv[3]) == 1) {

		cout << "\nEnter the number of processes: ";
		cin >> thread_count;

		clock_t t;

		t = clock();
		while(thread_count <= 0) {
			cout << "\nprocess count cannot be zero, enter another number: ";
			cin >> thread_count;
		}

		sub_section = total_lines/thread_count;
		extra_padding = total_lines - (thread_count * sub_section);


		//creating the processes.
		create_process();
		
		//waiting for the processes to get over.
		for(int i = 0; i < pid_v.size(); i++) {

			int status;

			waitpid(pid_v[i], &status, 0);
		}

		//joining all the files again.
		system("cat ag_axg9593* > ag_axg9593_lab_mergedfile");

		ifstream infile("ag_axg9593_lab_mergedfile");

		int numbers[8762] ;

		int num;
		int inc = 0;
		string line;
		while (getline(infile, line)) {

		    istringstream iss(line);
		    int a;
		    if (!(iss >> a)) { break; } // error

		    numbers[inc++] = a;
		}

		//sorting after reading all the sorted files.
		sort(numbers, numbers+total_lines);

		t = clock() - t;

		double ttime = ((double) t * 1000.0) / CLOCKS_PER_SEC;

		cout << "Total time taken : " << ttime << " ms" << endl;

		return 1;
	} else {

		start_index = atoi(argv[1]);
		number_of_lines = atoi(argv[2]);
	
		//reading the file for the values.s
		FILE* stream = fopen("all_month.csv", "r");

		//Checking if the file exists.
	    if(stream == NULL) {
	        cout << "[ERROR] The file does not exist. \n";
	        exit(0);
	    }

		double numbers[8762];
		char line[1024];

		int num;
		int inc = 0;
		int flag = 0;


		while (fgets(line, 1024, stream)){

	        //To remove the top row with the column identities. (i.e. TAG: Time, lat, lon)
	        if(flag == 0) {
	          flag = 1;
	          continue;
	        }

	        //reading only part of the file depending on the calling process.
	        if(--start_index > 0) {
		    	continue;
		    }else if(--number_of_lines < 0) {
		    	break;
		    }

	        char* tmp = strdup(line);

	        numbers[inc] = atof(gf(tmp, 5));
	        free(tmp);
	        inc = inc+1;
	    }

		string file_name;
		char numstr[21]; // enough to hold all numbers up to 64-bits
		sprintf(numstr, "%d", getpid());


		string pid_num(numstr);
		string name = "ag_axg9593_lab_" + pid_num;

		ofstream fs(name.c_str());

		//sorting and storing it in the file.
		sort(numbers, numbers+inc);

		for(int i = 0; i < inc; i++) {

			fs << numbers[i] << endl;
		}

		fs.close();

		return 0;
	}
}
