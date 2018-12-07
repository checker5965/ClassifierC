#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846
int testsize;

typedef struct node *nptr;

typedef struct dnode *dptr;

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

typedef struct dnode{
    struct dnode* next;
    struct dnode* prev;
    int f;
    float d;
} dnn;

dptr insertinorder(dptr head,float f,int nattr, float d)
{
    dptr curr = head;
    dptr p = malloc(sizeof(dnn));
    p->d = d;
    p->f = (int)f;
    if(curr==NULL)    //Insert at head is it is NULL
    {
        head = p;
        p->next = p;
        p->prev = p;
        return(head);
    }
    else if(d<=curr->d)   //Insert at head if distance of element to be inserted is  lower than that of head
    {
        p->next = head;
        p->prev = head->prev;
        (head->prev)->next=p;
        head->prev=p;
        return(p);
    }
    else
    {
        for(curr=head->next;curr!=head;curr=curr->next)   //Insert if element has to be placed between two elements based on distance
        {
            if(d<=curr->d)                          
            {
                p->next = curr;
                p->prev = (curr->prev);
                (curr->prev)->next=p;
                curr->prev = p;
                return(head);
            }
        }
        p->next = head;             //Insert at end if element distance is greater than every current distance
        p->prev = head->prev;
        (head->prev)->next = p;
        head->prev = p;
        return(head);
    }
}

int Kpredict(dptr head,int k,int c)
{
    int cl[c];                    //Array that will contain number of instances of each class among k nearest neighbours
    for(int i=0;i<c;i++)          //Initializing each element to 0
        cl[i]=0;
    
    for(int i=0;i<k;i++)
    {
        cl[head->f]+=1;          //Add 1 to the classification of nearest neighbour and move to the next neighbour
        head=head->next;
    }
    int max=0;                   //Check which class occurs the most
    for(int i=1;i<c;i++)
    {
        if(cl[i]>cl[max])
        {
            max=i;
        }
        else if(cl[i]==cl[max])    //If two classes have same number of instances, one is selected, with 0.5 probability of each
        {
            if(rand()>((0.5)*(RAND_MAX)))
            {
                max=i;
            }
        }
    }
    return(max);
}

int* knnclassifier(nptr test,nptr train,int nattr,int k, int c)
{
    int* predictions=malloc(sizeof(int)*testsize);                                                       //No. of classifications in the dataset
    nptr temp_test=test,temp_train=train;
    float distance;
    int i=0;
    do
    {
        int count=0;
        dptr distances=NULL;
        do
        {
            distance=0;
            for(int j=0;j<nattr-1;j++)   //Calculating distance of test case from each train case using Euclidian method
                distance+=sqrt(fabs(temp_train->f[j]-temp_test->f[j])*(temp_train->f[j]+temp_test->f[j]));
            distances=insertinorder(distances,temp_train->f[nattr-1],nattr,distance);   //Inserting train datapoints into a linked list based on their distance from the test datapoint
            temp_train=temp_train->next;
        }while(temp_train!=train);
        predictions[i]=Kpredict(distances,k,c);   //Predicting a class for the test dataset based on the first k elements of the linked list just created
        temp_test=temp_test->next;
        i++;
        
    }while(temp_test!=test);
    return(predictions);
}

nptr listcat(nptr head,nptr test, int nattr)
{
    if(test!=NULL)
    {
        nptr temp=test;
        do
        {
            head=insertAtEnd(head,temp->f,nattr);
            temp=temp->next;
        }while(test!=temp);
        return(head);
    }
}

void freeList(nptr head, int nattr)
{
    if(!head)
        return;
    nptr curr = head;
    nptr temp = head;
    while(temp!=head) //looping till head if found twice
    {
        curr = curr->next;
        free(curr);
    }
    free(head);
}

int main(void)
{
    srand ( time(NULL) );
    while(1)
    {
        int control;
        printf("Press 1: Naive Bayes Classifier\nPress 2: KNN Classifier\nPress 3: Exit\n\n");
        scanf("%d", &control);
        if(control == 1)
        {
            while(1)
            {
                printf("\nPress 1: Iris\nPress 2: Loan Prediction\nPress 3: Turkiye Student Evaluation\nPress 4: Pima Indians Diabetes\nPress 5: Trip Data\nPress 6: Exit\n\n");
                int dataN;
                scanf("%d", &dataN);
                char* filename;
                int nattr;
                int nclass;
                if(dataN==1)
                {
                    filename = "cleanIris.csv";
                    nattr = 5; 
                    nclass = 3;
                }
                else if(dataN==2)
                {
                    filename = "cleanBanker.csv";
                    nattr = 21; 
                    nclass = 2;
                }
                else if(dataN==3)
                {
                    filename = "gradePredict.csv";
                    nattr = 33; 
                    nclass = 5;   
                }
                else if(dataN==4)
                {
                    filename = "pid.csv";
                    nattr = 9; 
                    nclass = 2;   
                }
                else if(dataN==5)
                {
                    filename = "bikes.csv";
                    nattr = 4; 
                    nclass = 2;   
                }
                else if(dataN==6)
                {
                    break;
                }
                else
                {
                    printf("Wrong Choice. Enter Again.\n");
                    continue;
                }
                float AvAc = 0;
                nptr head;
                head = NULL;
                head = listify(filename, nattr, head);
                for(int y=1;y<=1000;y++)
                { 
                    nptr test;
                    test = NULL;
                    int testS = splitdata(&head, &test, nattr);
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
                    AvAc += (float)(testS-wCount)/(float)testS;
                    printf("\rPercentage Completed: %d%%",(y/10));
                    listcat(head, test, nattr);
                    freeList(test, nattr);
                    // nptr test;
                    // test = NULL;
                }
                printf("\nAccuracy: %g\n", AvAc/10);
            }
        }
        else if(control == 2)
        {
            while(1)
            {
                char* filename;
                int nattr;
                int nclass;
                printf("Press 1: Iris\nPress 2: Loan Prediction\nPress 3: Pima Indians Diabetes\nPress 4: Exit\n\n");
                int dataN;
                scanf("%d", &dataN);
                if(dataN==1)
                {
                    filename = "cleanIris.csv";
                    nattr = 5; 
                    nclass = 3;
                }
                else if(dataN==2)
                {
                    filename = "cleanBanker.csv";
                    nattr = 21; 
                    nclass = 2;
                }
                else if(dataN==3)
                {
                    filename = "pid.csv";
                    nattr = 9; 
                    nclass = 2;   
                }
                else if(dataN==4)
                {
                    break;
                }
                else
                {
                    printf("Wrong Choice. Enter Again.\n");
                    continue;
                }
                int k;                                     //K is entered by the user
                printf("Enter K: ");
                scanf("%d",&k);
                float accuracy=0;
                int counter=0;
                nptr train;          //Linked list that contains the train dataset(Entire dataset initially)
                train = NULL;
                train = listify(filename, nattr, train);
                for(int i=0;i<1000;i++)                   //Loop that runs the classifier 1000 times and calculates the mean accuracy
                {
                       //Creating linked list of the given dataset
                    nptr test; 
                    test=NULL;
                    testsize=splitdata(&train,&test,nattr);
                    int* predictions=malloc(sizeof(int)*testsize);
                    predictions=knnclassifier(test,train,nattr,k, nclass);    //Calling knnclassifier to storing predictions in an array
                    int correct=0;
                    nptr temp=test;
                    int i=0;
                    do                                              //Comparing predictions for test dataset with the actual classes of the test datapoints
                    {
                        if((int)temp->f[nattr-1]==predictions[i])
                            correct+=1;
                        i++;
                        temp=temp->next;
                    }while(temp!=test);
                    accuracy+=((float)correct)/testsize;
                    counter++;
                    printf("\rPercentage Completed: %d%%",(counter/10));
                    listcat(train, test, nattr);
                    freeList(test, nattr);   //Printing percentage of operation completed
                }
                printf("\nAccuracy= %g%%\n",(accuracy/10)); 
            }
        }
        else if(control==3)
        {
            exit(0);
        }
        else
        {
            printf("Wrong Input.\n");
            continue;
        }
    }
}