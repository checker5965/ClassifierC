#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

typedef struct node *nptr;

//struct
typedef struct node{
    struct node* next;
    struct node* prev;
    float* f;
} dll;

nptr insertAtEnd(nptr head, float *f, int nattr)
{
    nptr curr = head; 
    nptr p = malloc(sizeof(dll));
    p->f = malloc(sizeof(float)*nattr);
    p->f = f;
    if(curr==NULL)
    {
        head = p;
        p->next = p;
        p->prev = p;
        return(head);
    }
    else
    {
        p->next = curr;
        p->prev = curr->prev;
        (curr->prev)->next = p;
        curr->prev = p;
        return(head);
    }
}

void display(nptr head, int nattr)
{
    if(head == NULL)
    {
        printf("Empty.\n");
        return;
    }
    nptr temp = head;
    nptr curr = head;
    while(1) //looping till head if found twice
    {
        for(int i=0; i<nattr; i++)
            printf("%g, ", curr->f[i]);
        printf("\n");
        curr = curr->next;
        if(curr==temp)
            break; //breaking if head found second time
    }
}

nptr listify(char* filename, int nattr, nptr head)
{
    int flag = 0;
    FILE *dataset = fopen(filename, "r");
    while(1)
    {
        int c = 0;
        float* f = malloc(sizeof(float)*nattr);
        while(c!=nattr)
        {
            fscanf(dataset, "%g%*c", &f[c]);
            if(feof(dataset))
                flag = 1;
            c++;
        }
        head = insertAtEnd(head, f, nattr);
        if(flag)
            break;
    }
    return(head);
}

int splitdata(nptr* train, nptr* test, int nattr)
{
	nptr curr,temp;
    int c = 0;
	for(curr=(*train)->next;curr!=(*train);curr=curr->next)
	{
		if(rand()>((0.67)*(RAND_MAX)))
		{
			(*test)=insertAtEnd((*test),curr->f,nattr);
			(curr->next)->prev = curr->prev;
            (curr->prev)->next = curr->next;
            temp=curr;
            curr=curr->prev;
            free(temp);
            c++;
		}
	}
    return c;
}

double probability(double x, double mean, double stdev)
{
    double expo = exp(-(pow(x-mean,2)/(2*pow(stdev,2))));
    double numo = 1/(sqrt(2*PI)*stdev);
    return (numo*expo);    
}

double predict(int* predictions, nptr test, double** mean, double** stdev, int size, int nattr, int nclass)
{
    nptr curr = test;
    for(int i=0; i<size; i++)
    {
        double prob[nclass];
        for(int j=0; j<nclass; j++)
        {
            prob[j] = 1;
            for(int k=0; k<nattr-1; k++)
            {
                prob[j]=prob[j]*probability(curr->f[k], mean[j][k], stdev[j][k]);
                // if (prob[j]>1)
                //     printf("Something's wrong\n");
            }
        }
        int class = -1;
        double maxprob = -1;
        for(int l=0; l<nclass; l++)
        {
            if(class==-1 || prob[l]>maxprob)
            {
                class = l;
                maxprob = prob[l];
            }
        }
        predictions[i] = class;
        curr = curr->next;
    }   
}

void statistics(nptr head, int nattr, double* mean, double* variance, double* stdev, int class)
{
    if(head)
    {
        nptr temp = head;
        nptr curr = head;
        for(int i=0; i<nattr-1; i++)
        {
            double sum = 0; 
            double sumv = 0;
            int c = 0;
            while(1) //looping till head if found twice
            {
                c++;
                curr = curr->next;
                if(curr->f[nattr-1] != class)
                {
                    c--;
                    if(curr==temp)
                        break;
                    continue;
                }
                sum = sum+curr->f[i];
                if(curr==temp)
                    break; //breaking if head found second time
            }
            mean[i] = sum/c;
            c = 0;
            while(1) //looping till head if found twice
            {
                c++;
                curr = curr->next;
                if(curr->f[nattr-1] != class)
                {
                    c--;
                    if(curr==temp)
                        break;
                    continue;
                }
                sumv = sumv+pow((curr->f[i]-mean[i]), 2);
                if(curr==temp)
                    break; //breaking if head found second time
            }
            variance[i] = sumv/c;
            stdev[i] = sqrt(variance[i]);
            //printf("Mean: %g\nStDev%g\n\n", mean[i], stdev[i]);
        }
    }
}

int main(void)
{
    srand ( time(NULL) );
    nptr head;
    head = NULL;
    char* filename = "pid.csv";
    int nattr = 9; 
    int nclass = 2;
    nptr test;
    test = NULL;
    head = listify(filename, nattr, head);
    int testS = splitdata(&head, &test, nattr);
    printf("TEST SIZE: %d\n", testS);
    double** mean, **variance, **stdev;
    mean = malloc(sizeof(int));
    variance = malloc(sizeof(int));
    stdev = malloc(sizeof(int));
    for(int i=0; i<nclass; i++)
    {
        mean[i] = malloc(sizeof(double)*nattr);
        variance[i] = malloc(sizeof(double)*nattr);
        stdev[i] = malloc(sizeof(double)*nattr);
        statistics(head, nattr, mean[i], variance[i], stdev[i], i);
    }
    int* predictions = malloc(sizeof(int)*testS);
    predict(predictions, test, mean, stdev, testS, nattr, nclass);
    float error = 0;
    int wCount = 0;
    for(int i=0; i<testS; i++)
    {
        error = error + abs(test->f[nattr-1] - predictions[i]);
        if((int)test->f[nattr-1]!=(int)predictions[i])
            wCount++;
        test = test->next;
    }
    printf("Accuracy: %g\n", ((float)(testS-wCount)/(float)testS)*100.0);
    printf("MAE: %g\n", error/testS);
}