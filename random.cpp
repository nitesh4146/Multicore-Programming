#include <iostream>
#include <cstdlib>
//#include <ctime>
using namespace std;

int main(){
	//srand(time(0));
	int random[1000];
	for(int i=0; i<999; i++){
		random[i] = rand()%2;
		printf("Random array: %d\n", random[i]);
	}


	return 0;
}