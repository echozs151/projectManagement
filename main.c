/*
 * main.c
 *
 *  Created on: Apr 1, 2019
 *      Author: EYCO10
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define _D 26
#define TOTAL_JOBS 10
#define _SHIFT1 1
#define _SHIFT2 2
#define _SHIFT3 3

typedef struct
{
    int next[100];
    int totalNext;
    int prev[100];
    int totalPrev;
    int id;
    int cost;
    short status; // -1 not started,0 working, X day finished

} Nodeg;


char *INPUT1 = NULL,*INPUT2 = NULL,*INPUT3 = NULL;//"C:\\Users\\ECHOZS\\Desktop\\data\\bin\\ProjMng_Project\\data\\10_jobs_(manning).txt";
int setRule;
int assignedStaff[_D][3];
int freeStaff[_D][3];

int shiftCost[3];

int manning[70][70];
int network[70][70];
int times[70][70];

int shifts[60][3*_D];

int doneProject[100];

int nextProject[100];
int totalNextProject = 0;
int totalDoneProject = 0;

int mProfile = 2;
Nodeg jobs[70];

void printData(int dayss,int jobs)
{
	int days = _D;
	short placeholders = 0;
	//printf("\t\tshift 1 \t\tshift 2 \t\tshift 3\n");
	printf("\n\n");
	printf("Project\t\n");
	printf("--------------");
	for(int ii=0;ii<3;ii++){
		//if(days > 10){
		for(int iia=0;iia<days;iia++){
			printf("---");}
		//printf("-------------------------------------------------shift %i-------------------------------------------",ii+1);
		printf("-shift %i-",ii+1);
		//if(days > 10){
		printf("------");
		for(int iia=0;iia<days;iia++){
			printf("-----");}

	}
	printf("\n");
	for(int k=0;k<3;k++)
	{
		printf("\t\t");
		for(int kk=0;kk<days;kk++)
		{
			printf("%i\t",kk+1);
		}
		printf("  ");

	}
	printf("\n");
	for(int i=0;i<jobs;i++){
		printf("%i",i+1);
		printf("\t\t");
		for(int j=0;j<days*3;j++)
		{
			if(j % days == 0 && j != 0 )
				printf("\t\t");
			if(shifts[i][j] == 0 && placeholders == 0)
				printf(" \t");
			else
				printf("%i\t",shifts[i][j]);
		}
		printf("\n");
	}

}

void setShifts()
{
	for(int i=0;i<60;i++){
		for(int j=0;j<80;j++)
			shifts[i][j] = 0;
	}
	for(int i=0;i<3;i++)
	{
		shiftCost[i] = 0;
	}
}

int assumePeriod(int project,int duration,int manning)
{
	// find empty period.
	// if no available period then...
	// 		sum the cost of all 3 shifts. min(Sum(manning*shift_cost))
	//		compare which has lowest cost when bring new staff.
	//		bring new staff for min shift
	//		include cost to final cost
	//assign staff to period.
	if(project==1 || project==12 || project == 32 || project == 62)
		return 0;


	int k=0;
	int s,d;
	int j,jj,shiftj=0,shift;
	short soon=0;
	short free;
	short clearPrecedence = 0;
	int costSum[3];
	int lastDay = 0;
	int totalPrevious=0;
	int earliestShift = 0,earliestDay = 0;
	int previousFinish[20];
	for(int reset=0;reset<20;reset++)
	{
		previousFinish[reset] = 0;
	}
	for(int prev=0;prev<jobs[project-1].totalPrev;prev++)
	{
		if(jobs[project-1].prev[prev] == 1)
		{
			clearPrecedence = 1;
			earliestDay = 0;
			earliestShift = 0;
			break;
		}


		previousFinish[prev] = jobs[(jobs[project-1].prev[prev])-1].status;
		if(earliestDay <= jobs[(jobs[project-1].prev[prev])-1].status)
		{
			earliestDay = jobs[(jobs[project-1].prev[prev])-1].status;
			//earliestDay = earliestDay*(earliestShift+1);
		}
		if(earliestShift <= floor(jobs[(jobs[project-1].prev[prev])-1].status / _D))
		{
			earliestShift = floor(jobs[(jobs[project-1].prev[prev])-1].status / _D);
		}

		totalPrevious++;
	}

	if(earliestDay + duration > _D*3)
		return -10;

	while(k < duration)
	{
		soon = 0;
		free =0;
		j = k-1+times[project-1][2];
		jj = k+times[project-1][2];

		if(j > _D)
		{
			shiftj++;
			j=0;
		}
		if( jj <= _D)
			shift = 1;
		else if(jj > _D && jj <= _D*2)
			shift = 2;
		else if(jj > 2*_D && jj <= 3*_D)
			shift = 3;



		for(s=0;s<3;s++) // iterate through days
		{
			/*for(int prev=0;prev<jobs[project-1].totalPrev;prev++){

				if(previousFinish[prev] < d)
				{
					soon++;
				}
			}

			if(soon > 0 && clearPrecedence == 0)
				continue;*/
			for(d=0;d<_D;d++) // iterate through periods
			{
				if(freeStaff[d][s] > manning && s>=earliestShift && d*(s+1)>=earliestDay)//if(freeStaff[d][p] > 0)
				{
					free = 1;
					printf("Assigned free staff p: %i d: %i\n",s,d);
					break;
				}
			}
			if(free == 1)
			{
				printf("p: %i d: %i\n",s,d);
				freeStaff[d][s] -= manning;
				assignedStaff[d][s] += manning;
				shifts[project-1][d*(s+1)] = manning;
				if(d*(s+1)>lastDay)
					lastDay = d*(s+1);
				break;
			}

		}
		if(free == 0)
		{
			// bring new staff
			// implement all after *if no available period*

			// find shift with the lowest cost before bringing new staff
			int minCostShift= 9999;
			int minShift;
			int tempCost;
			for(s=0;s<3;s++)
			{
				tempCost = (s+1)*manning;
				if((shiftCost[s]+tempCost) <  minCostShift){
					minCostShift = shiftCost[s]+tempCost;
					minShift = s;
				}
			}
			//bring new staff
			for(int ns=0;ns<_D;ns++)
			{
				freeStaff[ns][minShift] += manning;
			}
			//shifts[project-2][minShift] = manning;
			shiftCost[minShift] += minCostShift;
			k--; // reverts the following k++ to take effect the newly brought staff into a shift
		}
		jobs[project-1].status = lastDay;


		printf("Project: %i k: %i p: %i d: %i\n",project,k,s,d);
		k++;
		j++;
	}
	printf("Final : p: %i d: %i\n",s,d);
	return 0;
}

int assumePeriod2(int project,int duration,int start,int manning)
{
	// make choice.
	int i,j;
	int k;
	int shift,shiftj = 1;
	int minCost = 9999;
	int minp;
	int cost=0;
	int costModifier = _SHIFT1;
	int trackShift = start;
	if(project==1 || project==12 || project == 32 || project == 62)
		return 0;
	// reserve earliest available period.




	/*for(i=1;i<=duration;i++)
	{
		if( i <= _D)
				shift = 1;
			else if(i > _D && i <= _D*2)
				shift = 2;
			else if(i > 2*_D && i <= 3*_D)
				shift = 3;
	}*/

	i=1;
	while(i<=duration)
	{
		k = i+start;
		j = i-1+start;
		if(j > _D)
		{
			shiftj++;
			j=0;
		}
		if( k <= _D)
			shift = 1;
		else if(k > _D && k <= _D*2)
			shift = 2;
		else if(k > 2*_D && k <= 3*_D)
			shift = 3;

		if(freeStaff[i-1][shift-1] == 0)
		{
			for(int j=0;j<_D;j++)
			{
				if(shift == 1)
					costModifier = _SHIFT1;
				else if(shift == 2)
					costModifier = _SHIFT2;
				else if(shift == 3)
					costModifier = _SHIFT3;
				freeStaff[j][shift-1] += manning;

			}
			cost += manning * shift;
		}


		shifts[project-2][start+i-1] = manning;

		freeStaff[j][shiftj-1] -= manning;

		assignedStaff[i-1][shift-1] += manning;
		trackShift++;

		i++;
		j++;
	}



	jobs[project-1].status = k;
	printf("cost mod: $%i\n",costModifier);
	return cost;
}


int assumePeriod1(int project,int duration,int es,int manning)
{

	int cost=0;
	int costModifier = _SHIFT1;
	int trackShift = es;

	for(int i=es;i<es+duration;i++)
	{

		shifts[project-2][i] = manning;
		trackShift++;
	}
	return cost;
}

void lci()
{
	// network[0][x>1 until network[0][x] == -1] holds projects without predecessor
	// TODO lci algorithm
	int error = 0;
	int errorProject;
	int ii=0;
	int k=2;
	int projectId;
	int es = 9999,hManning = -1;
	int esProject,hManningProj;

	int totalCost=0;
	int tie=0;
	int nextProjCount=0;
	int nextPick = 0;

	int totalDuration=0;
	// initialize arrays
	for(int zero=0;zero<100;zero++)
	{
		doneProject[zero] = 0;
		nextProject[zero] = 0;
	}

	// Here are all the next nodes of the 1st dummy
	while(network[0][k] != - 1)
	{
		projectId = network[0][k];
		nextProject[totalNextProject] = projectId;
		totalNextProject++;
		k++;
	}



	// step 6
	for(;;)
	{
		k=0;
		nextProjCount=0;
		es = 9999;
		hManning = -1;
		ii=0;
		tie=0;
		while(totalNextProject != nextProjCount )
		{
			if(nextProject[k] != 0)
				nextProjCount++;
			else
			{
				k++;
				continue;
			}
			projectId = nextProject[k];
			if(times[projectId-1][2] <= es)
			{
				if(times[projectId-1][2] == es)
					tie++;
				es = times[projectId-1][2];
				esProject = projectId;

			}

			k++;
		}
		nextPick = esProject;
		nextProjCount=0;
		k=0;
		if(tie > 1)
		{
			while(totalNextProject != nextProjCount)
			{
				if(nextProject[k] != 0)
					nextProjCount++;
				else
				{
					k++;
					continue;
				}

				projectId = nextProject[k];
				if(manning[projectId-1][mProfile] > hManning)
				{
					hManningProj = projectId;
					hManning = manning[projectId][mProfile];
				}

				k++;
			}
			nextPick = hManningProj;
		}

		// step 3
		//if((nextProject[ii] == hManningProj && tie>1) || (nextProject[ii] == esProject && tie<=1))

		int finishedProject = nextPick;



		// Assign project into shifts
		totalCost = assumePeriod(nextPick,times[nextPick-1][1],manning[nextPick-1][mProfile]);

		if(totalCost == -10)
		{
			error = totalCost;
			errorProject = nextPick;
			break;

		}



		while(nextProject[ii] != nextPick)
			ii++;
		nextProject[ii] = 0;
		totalNextProject--;

		doneProject[totalDoneProject] = finishedProject;
		totalDoneProject++;

		// check for next job
		for(int nexti=0;nexti<jobs[finishedProject-1].totalNext;nexti++)
		{
			int nextJobTemp = jobs[nextPick-1].next[nexti];
			int countPrevious=0;
			for(int previ=0;previ<jobs[nextJobTemp-1].totalPrev;previ++)
			{
				for(int donei=0;donei<totalDoneProject;donei++)
				{
					if(jobs[nextJobTemp-1].prev[previ] == doneProject[donei])
					{
						countPrevious++;
						if(countPrevious == jobs[nextJobTemp-1].totalPrev)
						{
							for(int emptynext=0;emptynext<100;emptynext++)
							{
								if(nextProject[emptynext] == 0)
								{
									nextProject[emptynext] = nextJobTemp;
									totalNextProject++;
									break;
								}
							}
							countPrevious=0;
						}
					}
				}
			}
		}
		if(totalNextProject == 0)
			break;
	}
	//
	// int project,int duration,int es,int manning
	//
	if(error == -10)
		printf("\n***Project: %i Could not be scheduled***\n",errorProject);
	printf("done lci..\n");
	for(int printShift=0;printShift<3;printShift++)
	{
		printf("Shift %i Cost: %i\n",printShift+1,shiftCost[printShift]);
	}
}

void buildMap()
{
	int j,k;
	// Set nodes array
	for(int i=0;i<70;i++)
	{
		Nodeg temp;
		temp.id = 0;
		temp.totalNext = 0;
		temp.totalPrev = 0;
		temp.cost = 0;
		temp.status = -1;
		for(int r=0;r<100;r++)
		{
			temp.next[r] = 0;
			temp.prev[r] = 0;
		}
		jobs[i] = temp;
	}
	for(int i=0;i<_D;i++)
	{
		assignedStaff[i][0] = 0;
		assignedStaff[i][1] = 0;
		assignedStaff[i][2] = 0;
	}
	for(int s=0;s<3;s++)
		{
		for(int i=0;i<_D;i++){
			freeStaff[i][s] = 0;

		}}
	j=0;k=0;
	while(network[j][0] != -1)
	{
		k=0;
		if(network[j][k] != -1)
		{
			jobs[j].id = network[j][0];
			jobs[j].totalNext = network[j][1];
		}

		while(network[j][k] != -1)
		{
			if(k>1)
			{
				jobs[j].next[k-2] = network[j][k];
				jobs[network[j][k]-1].prev[jobs[network[j][k]-1].totalPrev] = jobs[j].id;
				jobs[network[j][k]-1].totalPrev++;
			}

			//printf("network[%i][%i]:%i \n",j,k,network[j][k]);
			k++;
		}
		if(jobs[j].totalNext == 0)
			break;
		j++;
	}


}

void readFileLine(int option)
{

    //char *input = "C:\\Users\\ECHOZS\\Desktop\\data\\bin\\ProjMng_Project\\data\\10_jobs_(manning).txt";

    FILE * fp1,* fp2,* fp3;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineCount =0;


	INPUT1 = "data\\10_jobs_(manning).txt";
	INPUT2 = "data\\10_jobs_(network).txt";
	INPUT3 = "data\\10_jobs_(times).txt";

	if(option == 30)
	{
		INPUT1 = "data\\30_jobs_(manning).txt";
		INPUT2 = "data\\30_jobs_(network).txt";
		INPUT3 = "data\\30_jobs_(times).txt";

	}else if(option == 60){
		INPUT1 = "data\\60_jobs_(manning).txt";
		INPUT2 = "data\\60_jobs_(network).txt";
		INPUT3 = "data\\60_jobs_(times).txt";

	}


    fp1 = fopen(INPUT1, "r");
    fp2 = fopen(INPUT2, "r");
    fp3 = fopen(INPUT3, "r");
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
        exit(EXIT_FAILURE);




    while ((read = getline(&line, &len, fp1)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
    	//if(lineCount>=1)
        //printf("%s",line);
    	//printf("%s",line);
        int i = 0;
        char *p = strtok (line, " \t");
        char *array[20];


        char *lineStore = &line;


            //Manning

                if(lineCount>=1)
                {
                    //printf("%s	%s", array[0],array);
                    //printf("%s",line);
                    //char *charArray[20];
                    //charArray = *strtok(line,"\t");
                    while (p != NULL)
                    {
                        array[i++] = p;
                        p = strtok (NULL, " \t");
                    }

                    if(lineCount >= 2)
                    {
						for(int i=0;i<11;i++)
						{
							manning[lineCount-2][i] = atoi(array[i]);

						}
                    }

                }

        lineCount++;
    }

    lineCount = 0;
    while ((read = getline(&line, &len, fp2)) != -1) {
		printf("%s",line);
		int i = 0;
		char *p = strtok (line, " \t");
		char *array[20];
		int lengthNet=0;
		char *lineStore = &line;
		// Network
		if(lineCount>=1)
		{
			//printf("%s	%s", array[0],array);
			//printf("%s",line);
			//char *charArray[20];
			//charArray = *strtok(line,"\t");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok (NULL, " \t");
			}
			if(lineCount >= 5)
			{
				for(int i=0;i<10;i++)
				{
					if(lengthNet <= atoi(array[1])+1)
						network[lineCount-5][i] = atoi(array[i]);
					if(lengthNet > atoi(array[1])+1 )
					{
						network[lineCount-5][i] = -1;
						break;
					}
					lengthNet++;
				}
			}

		}
		lineCount++;
    }

    lineCount = 0;
	while ((read = getline(&line, &len, fp3)) != -1) {
		//printf("Retrieved line of length %zu:\n", read);
		//if(lineCount>=1)
		//printf("%s",line);
		//printf("%s",line);
		int i = 0;
		char *p = strtok (line, " \t");
		char *array[20];

		char *lineStore = &line;
		//Manning
		if(lineCount>=1)
		{
			//printf("%s	%s", array[0],array);
			//printf("%s",line);
			//char *charArray[20];
			//charArray = *strtok(line,"\t");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok (NULL, " \t");
			}
			if(lineCount >= 4)
			{
				for(int i=0;i<3;i++)
				{
					times[lineCount-4][i] = atoi(array[i]);


				}
			}
		}
		lineCount++;
	}
	setShifts();
	//if(_D*3 >= times[TOTAL_JOBS+1][2])

		buildMap();
		lci();

		printData(_D,10+2);



	//	printf("Cannot fit workload..Increase Shifts or Deadline\n");


    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    if (line)
        free(line);
    //exit(EXIT_SUCCESS);

}



int main()
{

	setbuf(stdout, NULL);
	printf("1. LTT\n2. STT\n3. MFT\n4. LFT\n");
	printf( "Option :");
    for(;;)
    {

        int opt=0;

        //scanf("%d",&opt);
        //setRule = opt;

        readFileLine(opt);
        getchar();

        //printf( "Option :");

    }

    return 0;

}
