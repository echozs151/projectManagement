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

typedef struct
{
    int next[100];
    int totalNext;
    int prev[100];
    int totalPrev;
    int id;

} Nodeg;


char *INPUT1 = NULL,*INPUT2 = NULL,*INPUT3 = NULL;//"C:\\Users\\ECHOZS\\Desktop\\data\\bin\\ProjMng_Project\\data\\10_jobs_(manning).txt";
int setRule;

int manning[70][70];
int network[70][70];
int times[70][70];
int mProfile = 1;
Nodeg nodes[70];

void printData(){
	printf("data");
}

void lci()
{
	// network[0][x>1 until network[0][x] == -1] holds projects without predecessor
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
		for(int r=0;r<100;r++)
		{
			temp.next[r] = 0;
			temp.prev[r] = 0;
		}
		nodes[i] = temp;
	}
	j=0;k=0;
	while(network[j][0] != -1)
	{
		k=0;
		if(network[j][k] != -1)
		{
			nodes[j].id = network[j][0];
			nodes[j].totalNext = network[j][1];
		}

		while(network[j][k] != -1)
		{
			if(k>1)
				nodes[j].next[k-2] = network[j][k];

			printf("network[%i][%i]:%i \n",j,k,network[j][k]);
			k++;
		}
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
                    //printf("\n");
                    //printf("%s %\n",array[0]);
                    //char graphDirectory[100] = "data\\precedence graphs\\";
                    //strupr(array[0]);
                    //char graphName[40];// = array[0];
                    //strcpy(graphName,array[0]);
                    //strcat(graphName,".IN2");
                    //strcat(graphDirectory,graphName);

                    //char *graphDirectory = strcat(,graphName);
                    //printf("%s\n",graphDirectory); // Prints the path of the graph data
                    //printf("%s ",array[0]);





					//printf("\t%i\t %i\t",atoi(array[1]),atoi(array[2]));

                        //exit(EXIT_FAILURE);

                    //char *array2[800];

                    //double totalNodes = atoi(array2[0]);

                    //printf("%s	%s", array[0],array);
                    //printf("before alpbe");
                    //alpbe(atoi(array[1]),atoi(array[2]),array2);



                }

            //char *precedenceGraphName = array[0];

        lineCount++;
        //read only 5 lines for debug
        /*if(lineCount>5)
            break;*/
    }

    lineCount = 0;
    while ((read = getline(&line, &len, fp2)) != -1) {
		//printf("Retrieved line of length %zu:\n", read);
		//if(lineCount>=1)
		//printf("%s",line);
		printf("%s",line);
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
			if(lineCount >= 5)
			{
				for(int i=0;i<10;i++)
				{
					network[lineCount-5][i] = atoi(array[i]);
					if(atoi(array[i]) == NULL)
					{
						network[lineCount-5][i] = -1;
						break;
					}

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

	buildMap();

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

        int opt;

        //scanf("%d",&opt);
        //setRule = opt;

        readFileLine(opt);
        getchar();

        //printf( "Option :");

    }

    return 0;

}
