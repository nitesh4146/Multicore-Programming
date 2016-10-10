#include <iostream>
#include <cstdlib>
//#include <ctime>
using namespace std;
void readFile();

int main(){
	//srand(time(0));
	// FILE *fp;
	int count = 0;
	//fp = fopen("/home/nitish/Workspace/a2/random_1", "w+");
	int random[1000000];
	for(int i=0; i<1000000; i++){
		random[i] = rand()%2;
		//printf("Random array: %d\n", random[i]);
		//fprintf(fp, "%d\n", random[i]);
	}

	for (int i = 0; i < 1000000; i++)
    {
    	if(random[i]==0){
    		count++;
    	}
        //printf("Number is: %d\n\n", numberArray[i]);
    }
    printf("Total 1's are %d\n", count);
	//fclose(fp);
	//readFile();

	return 0;
}

void readFile(){
	FILE *fp;
    fp = fopen("/home/nitish/Workspace/a2/random_1", "r");
    int count=0;
    //read file into array
    int numberArray[1000000];
    int i;

    for (i = 0; i < 1000000; i++)
    {
        fscanf(fp, "%d", &numberArray[i]);
    }

    for (i = 0; i < 1000000; i++)
    {
    	if(numberArray[i]==0){
    		count += numberArray[i];
    	}
        //printf("Number is: %d\n\n", numberArray[i]);
    }
    printf("COunt of 1's: %d\n", count);
}