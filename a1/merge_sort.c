
#include <stdio.h>
 long merge_sort(long[],long[],int,int);
 
int main(){
	long a1[3] = {1,2,3};
	long a2[4] = {6,7,8,9};
	long *p,a3[7];
	p = merge_sort(a1,a2,3,4);
	
}

long merge_sort(long array1[], long array2[], int m, int n)
{
	long array3[7];
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < m && j < n)
    {
        if (array1[i] < array2[j])
        {
            array3[k] = array1[i];
            i++;
        }
        else
        {
            array3[k] = array2[j];
            j++;
        }
        k++;
    }
    if (i >= m)
    {
        while (j < n)
        {
            array3[k] = array2[j];
            j++;
            k++;
        }
    }
    if (j >= n)
    {
        while (i < m)
        {
            array3[k] = array1[i];
            i++;
            k++;
        }
    }
    printf("\n After merging: \n");
    for (i = 0; i < m + n; i++)
    {
        printf("\n%ld", array3[i]);
    }
    return array3;
}
