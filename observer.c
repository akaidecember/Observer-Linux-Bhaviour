/*********************************************************************************************************************************
A program writen in C language to explore and observe Linux behaviour. This program shows us the characteristics of the              Linux Kernel, processes, memory and other resources. For this, we have accessed the /proc file where all the                        information lies.

Created by: Anshul Kumar Shandilya
CSC 139, 
*********************************************************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>

enum REPORT_TYPE{STANDARD, SHORT, LONG};                //enum for creating the report types
struct timeval now;                                     //Timeval now for generating the current time

void printTime(char*, double);                          //Function prototype for the function printTime

//Function to fulfill the Part B requirements of the assignment
void partB(){
    
    //Variables
    double upTime = 0.0, idle = 0.0;
    FILE *File;
    char buffer[4000];                                  //Creating array of characters for a string/ buffer for reading the file
    
    //opening the file
    File = fopen("/proc/cpuinfo", "r");
    
    //Reading the file for vendor
    fgets(buffer, 4001, File);
    fgets(buffer, 4001, File);
    printf("%s", buffer);
    
    //Reading  the file for model Name
    fgets(buffer, 4001, File);
    fgets(buffer, 4001, File);
    fgets(buffer, 4001, File);
    printf("%s", buffer);
    fclose(File);                                       //Closing the file
    
    File = fopen("/proc/version", "r");                 //Accessing the /version file for version
    fgets(buffer, 4001, File);
    printf("%s", buffer);
    fclose(File);                                       //Closing file
        
    File = fopen("/proc/uptime", "r");                  //uptime file for knowing the uptime of the computer
    fscanf(File, "%lf %lf\n", &upTime, &idle);
    printTime("Time elapsed since last boot :", upTime);
    fclose(File);                                       //Closing the file
    
    gettimeofday(&now, NULL);                           //Current time
    printf("Current Time is : %s", ctime(&(now.tv_sec)));
    
    File = fopen("/proc/sys/kernel/hostname", "r");     //File for the hostname
    fgets(buffer, 4001, File);
    printf("The host of the machine is : %s", buffer);
    fclose(File);
    
}

//Function to fulfill the Part C requirements of the assignment
void partC(){
    
    //Initial variables declerations
    FILE *File, *File2;
    char buffer[4000], *str, *tempStr;
    long user = 0, sys = 0, temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0, temp7 = 0, temp8 = 0, read = 0, write = 0;
    int count = 0, i = 0;
    long long cSwitch = 0, idle = 0, processes = 0;
    
    File = fopen("/proc/stat", "r");                     //Opening the file /stat
    fscanf(File, "%s %lu %lu %lu %llu \n", &tempStr, &user, &temp1, &sys, &idle);
    printf("User mode: %lu\nSystem mode : %lu\nIdle mode : %llu\n", user, sys, idle);

    fseek(File, 0L, SEEK_SET);
    
    //Opening the diskstats file for information about the read and write factors
    File2 = fopen("/proc/diskstats","r");

    fgets(buffer, 4001, File2);  
    str = strstr(buffer, "sda");                        //Function to check if the line read contains 'sda' in it
    while(!str){                                        //Running a while loop to do above process if line does not have 'sda'
        
        fgets(buffer, 4001, File2);
        str = strstr(buffer, "sda");
        count++;
        
    }
    
    fseek(File2, 0L, SEEK_SET);                          //Setting the pointer to the beginning
    
    for(i=0;i<count; i++)                               //Here, we first reset the file, then bring the buffer to the same line at which we found 'sda'
        fgets(buffer, 4001, File2);
    
    fscanf(File2, "%lu %lu %s %lu %lu %lu %lu %lu %lu %lu\n", &temp1, &temp2, &temp3, &temp4, &temp5, &read, &temp6, &temp7, &temp8, &write);
    
    //printing the number of read and write requests
    printf("The number of Disk Read Requests are : %lu\nThe number of Disk Write Requests are : %lu\n", read, write);
    
    fclose(File2);                                      //closing the file
    
    //To determine the number of context switches
    fgets(buffer, 4001, File);
    str = strstr(buffer, "ctxt");
    count = 0;
    i = 0;
  
    //loop to reach the part of the file where ctxt lies
    while(!str){
        
        fgets(buffer, 4001, File);
        str = strstr(buffer, "ctxt");
        count++;
        
    }
    
    fseek(File, 0L, SEEK_SET);                              //resetting the file pointer
    
    for(i = 0; i < count; i++)
        fgets(buffer, 4001, File);
    
    fscanf(File, "%s %llu\n", &temp1, &cSwitch);
    
    //Printing the number of context switches made
    printf("The number of context switches made are : %lu\n", cSwitch);
    
    fseek(File, 0L, SEEK_SET);                              //resetting the file pointer
    
    //To determine the number of active processes in the OS
    fgets(buffer, 4001, File);
    str = strstr(buffer, "processes");
    
    //loop to reach the part of the file where processes lies
    while(!str){
        
        fgets(buffer, 4001, File);
        str = strstr(buffer, "processes");
        count++;
      
    }
    
    fseek(File, 0L, SEEK_SET);                              //resetting the file pointer
    
    for(i = 0 ; i < count; i++)
        fgets(buffer, 4001, File);
    
    fscanf(File, "%s %llu\n", &temp1, &processes);
    
    //printing the number of processes
    printf("The number of processes are : %llu\n", processes);
    
    fclose(File);                                           //closing the file
    
}

//Function to fulfill the Part D requirements of the assignment
void partD(int interval, int duration){
    
    //Initial variable declerations
    float avg = 0.0;
    FILE *File;
    char buffer[4000];
    int counter = 0;
    
    File = fopen("/proc/meminfo", "r");
    fgets(buffer, 4001, File);
    
    //Printing the total memory configured
    printf("The total memory is : %s\n", buffer);
    
    fseek(File, 0L, SEEK_SET);                              //resetting the file pointer
    
    fgets(buffer, 4001, File);
    fgets(buffer, 4001, File);
    
    //printing the memory that is available or free
    printf("The memory available is : %s\n", buffer);
    
    fclose(File);                                           //closing the file
    
    //loop to calculate average based on given interval and size
    do{
        
        if(counter != 0)
            sleep(interval);

        File = fopen("/proc/loadavg", "r");
        fscanf(File, "%f\n", &avg);
        printf("The load average is : %0.2f\n", avg);

        fclose(File);
        
        //incrementing counter to check for interval size given by the user
        counter += interval;
    
    }while(counter < duration);
    
}

//Function to print out the given number of seconds into dd:hh:mm:ss format
void printTime(char* label, double oldTime){
    
    long time = (long)oldTime;
    printf("%s : %ld:%ld:%ld:%ld\n", label, time/86400, (time % 86400)/ 3600, (time %3600)/60, time % 60);
    
}

//The main function of the program
int main(int argc, char *argv[]){
    
    int interval, duration;                             //These variables are for command line arguments
    char repTypeName[16], c1, c2;
    enum REPORT_TYPE reportType;
    
    //Determine report type
    reportType = STANDARD;
    strcpy(repTypeName, "STANDARD");
    
    if (argc > 1) {
        sscanf(argv[1], "%c%c", &c1, &c2);
        
        if (c1 != '-') {
            fprintf(stderr, "usage: observer [-s][-l int dur]\n");
            exit(1);
        }
        
        if (c2 == 's') {
            reportType = SHORT;
            strcpy(repTypeName, "Short");
        }
        
        if (c2 == 'l') {
            reportType = LONG;
            strcpy(repTypeName, "Long");
            interval = atoi(argv[2]);
            duration = atoi(argv[3]);
        }
    }
    
    //To conditionally print out the desired report, namely Standard, Short and Long
    //Is goint to run if no arguments
    if(reportType == STANDARD)                          
        partB();
    else if(reportType == SHORT){                               //If one argument given
        
        partB();
        partC();
        
    }
    else{                                                       //If two arguments given
        
        partB();
        partC();
        partD(interval, duration);
        
    }
        
}