#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// declare global variables including a table structure to hold scheduling information


struct node
{
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int already_started;
    int end_time;
    int turnaround_time;
} *table_ptr = NULL;

typedef struct node table_type;

int num_processes;
int num_done;

// optional: define a function that finds the maximum of two integers
#define max(a,b) (a > b) ? a : b


//***************************************************************
void printTable() {
	// declare local variables
	// print table header 
	printf("PID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");
	printf("--------------------------------------------------\n");
	
	// for each process
	for(int i = 1; i <= num_processes; i++)
	{
	    // print the contents (id, arrival time, total_cycles) of each field of the table's index 
	    printf("%d\t", table_ptr[i].id);
	    printf("%d\t", table_ptr[i].arrival);
	    printf("%d\t", table_ptr[i].total_cpu);
	    
	    // if process has been scheduled ("done" field is 1, print other contents (start time, end time, turnaround time) 
	    if(table_ptr[i].done == 1)
	    {
	        printf("%d\t", table_ptr[i].start_time);
	        printf("%d\t", table_ptr[i].end_time);
	        printf("%d\t", table_ptr[i].turnaround_time);
	    }
	    
	    printf("\n");
	}
	printf("\n");
	return;
}


//*************************************************************
void enterParameter() {
	// declare local variables 
	
	// prompt for total number of processes
	printf("Enter total number of processes: ");
	scanf("%d", &num_processes);
	
	// allocate memory for table to hold process parameters
	table_ptr = (table_type*)calloc((num_processes + 1) , sizeof(table_type));
	
	// for each process
	for (int i = 1; i <= num_processes; i++)
	{
	    table_ptr[i].id = i;
	    
	    // prompt for arrival time, and total cycle time 
	    printf("Enter arrival cycle for process %d: ", i);
	    scanf("%d", &table_ptr[i].arrival);
	    printf("Enter total cycles for process %d: ", i);
	    scanf("%d", &table_ptr[i].total_cpu);
	}
			
	// print contents of table 
	printTable();
	return;		
}	
		

//*************************************************************
void fifo() {
	// declare (and initilize when appropriate) local variables 
	int earliest_arrival;
	int min_index;
	int current_cycle = 0;
	
	num_done = 0;
	
	// for each process, reset "done" field to 0 
	for(int i = 1; i <= num_processes; i++)
	{
	    table_ptr[i].done = 0;
	}
	
	// while there are still processes to schedule 
	while(num_done < num_processes)
	{
	    // initilize the earliest arrival time to INT_MAX (largest integer value) 
	    earliest_arrival = INT_MAX;
	    
		// for each process not yet scheduled 
		for(int i = 1; i <= num_processes; i++)
		{
		    if(table_ptr[i].done == 0)
		    {
                // check if process has earlier arrival time than current earliest and update 	
		        if (table_ptr[i].arrival < earliest_arrival)
		        {
		            earliest_arrival = table_ptr[i].arrival;
		            min_index = i;
		        }
		    }
		}
		
		if(current_cycle < earliest_arrival)
		{
		    current_cycle = earliest_arrival;
		}
		
		// set start time, end time, turnaround time, done fields for unscheduled process with earliest arrival time        	
		table_ptr[min_index].start_time = current_cycle;
		current_cycle = current_cycle + table_ptr[min_index].total_cpu;
		table_ptr[min_index].end_time = current_cycle;
		table_ptr[min_index].turnaround_time = current_cycle - table_ptr[min_index].arrival;
		table_ptr[min_index].done = 1;
		
		
		// update current cycle time and increment number of processes scheduled 
		num_done++;
	}
	
	// print contents of table
	printTable();
	return;		
}	


//*************************************************************
void sjf() {
	// declare (and initilize when appropriate) local variables 
	int shortest_total;
	int current_cycle = 0;
	int min_index;
	num_done = 0;
	
	// for each process, reset "done" field to 0 
	for(int i = 1; i <= num_processes; i++)
	{
	    table_ptr[i].done = 0;
	}
	
	// while there are still processes to schedule 	
	while(num_done < num_processes)
	{
		// initilize the lowest total cycle time to INT_MAX (largest integer value) 
	   shortest_total = INT_MAX;
		
		// for each process not yet scheduled
		for(int i = 1; i <= num_processes; i++)
		{
		    if(table_ptr[i].done == 0)
		    {
                // check if process has lower total cycle time than current lowest and has arrival time less than current cycle time and update 
                if(table_ptr[i].total_cpu < shortest_total && table_ptr[i].arrival <= current_cycle)
                {
                    shortest_total = table_ptr[i].total_cpu;
                    min_index = i;
                }
		    }
		}
		
		if(shortest_total == INT_MAX)
		{
		    current_cycle++;
		}
		else
		{
		    // set start time, end time, turnaround time, done fields for unscheduled process with lowest (and available) total cycle time    
		    table_ptr[min_index].start_time = current_cycle;
		    current_cycle = current_cycle + table_ptr[min_index].total_cpu;
		    table_ptr[min_index].end_time = current_cycle;
		    table_ptr[min_index].turnaround_time = current_cycle - table_ptr[min_index].arrival;
		    table_ptr[min_index].done = 1;
		    // update current cycle time and increment number of processes scheduled
		    num_done++;
		}
		 
	}
	// print contents of table 
	printTable();
	return;		
}	
        	

//*************************************************************
void srt() {
	// declare (and initilize when appropriate) local variables 
	int lowest_remaining;
	int current_cycle = 0;
	int min_index;
	
	num_done = 0;
	
	// for each process, reset "done", "total_remaining" and "already_started" fields to 0 
	for(int i = 1; i <= num_processes; i++)
	{
	    table_ptr[i].done = 0;
	    table_ptr[i].total_remaining = table_ptr[i].total_cpu;
	    table_ptr[i].already_started = 0;
	}
	
	// while there are still processes to schedule
	while(num_done < num_processes)
	{
		// initilize the lowest total cycle time to INT_MAX (largest integer value) 
		lowest_remaining = INT_MAX;
		
		// for each process not yet scheduled
		for(int i = 1; i <= num_processes; i++)
		{
		    if(table_ptr[i].done == 0)
		    {
                // check if process has lower total cycle time than current lowest and has arrival time less than current cycle time and update 
                if(table_ptr[i].total_remaining < lowest_remaining && table_ptr[i].arrival <= current_cycle)
                {
                    lowest_remaining = table_ptr[i].total_remaining;
                    min_index = i;
                }
		    }
		}
		
		if(lowest_remaining == INT_MAX)
		{
		    current_cycle++;
		}
		else
		{
		    // check if process already partially-scheduled
		    if(table_ptr[min_index].already_started == 0)
		    {
		        // if so, set "start time", "already_started" fields of process with lowest (and available) total remaining cycle time 
		        table_ptr[min_index].already_started = 1;
		        table_ptr[min_index].start_time = current_cycle;
		    }
		    
		    // decrement total remaining time of process with lowest (and available) total remaining cycle time 
		    table_ptr[min_index].total_remaining = table_ptr[min_index].total_remaining -1;
		    current_cycle++;
			
		    // set end time, turnaround time of process with lowest (and available) total remaining cycle time 
		    if(table_ptr[min_index].total_remaining == 0)
		    {
		        table_ptr[min_index].end_time = current_cycle;
		        table_ptr[min_index].turnaround_time = table_ptr[min_index].end_time - table_ptr[min_index].arrival; 
		        table_ptr[min_index].done = 1;
		        num_done++;
		    }

	    	
	
		}
	}
		// print contents of table
		printTable();
	return;		
}	
        	

//*************************************************************
void freeTable() {
	// free the schedule table if not NULL 
	if(table_ptr != NULL)
	{
	    free(table_ptr);
	}
	return;
}


//*************************************************************
int main() {
	// declare local vars 
	int choice = -1;
	while(choice != 5)
    {
        /* print menu of options */

        printf("Batch scheduling\n");
        printf("----------------\n");
        printf("1) Enter parameters\n");
        printf("2) Schedule processes with FIFO algorithm\n");
        printf("3) Schedule processes with SJF algorithm\n");
        printf("4) Schedule processes with SRT algorithm\n");
        printf("5) Quit and free memory\n");
        
        // prompt for menu selection 
        printf("\nEnter selection: ");
        scanf("%d", &choice);
        
        printf("\n");
        
        /* call appropriate procedure based on choice--use switch statement or series of if, else if, else statements */
        switch(choice)
        {
            case 1: 
                enterParameter();
                break;
            case 2:
                fifo();
                break;
            case 3:
                sjf();
                break;
            case 4:
                srt();
                break;
            case 5:
                freeTable();
        }
        
    }
    
    
   printf("\nQuitting program...");
    
    
	 return 1; // indicates success 
 }// end of procedure 
 



