#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

//Global variable to store the number of test cases
int testsize;

typedef struct node *nptr;

//Linked list structure to store the train and test datasets 
typedef struct node{
    struct node* next;
    struct node* prev;
    float* f;
} dll;

typedef struct dnode *dptr;

//LInked list structure to store the distance of each train case from the test cases
typedef struct dnode{
    struct dnode* next;
    struct dnode* prev;
    int f;
    float d;
} dnn;

//Function to insert a new element at the ened of a linked list
nptr insertAtEnd(nptr head, float* f, int nattr)
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

//Function to insert attributes and distance of each train case from a test case sorted by distance
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

//Function to display linked list(Used only when a check is needed)
void display(dptr head)
{
    if(head == NULL)
    {
        printf("Empty.\n");
        return;
    }
    dptr temp = head;
    dptr curr = head;
    while(1) //looping till head if found twice
    {
        printf("%d, ", curr->f);
        printf("%f",curr->d);
        printf("\n");
        curr = curr->next;
        if(curr==temp)
            break; //breaking if head found second time
    }
}

//Function to create a linked list from the given dataset
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

//Function to split linked list of the entire dataset to test and train based on the given ratio
int splitdata(nptr* train, nptr* test, int nattr,float split)             //(IMP) Need to create test linked list first and initialize to null
{
	int count=0;
	nptr curr,temp;
	for(curr=(*train)->next;curr!=(*train);curr=curr->next)
	{
		if(rand()>((0.8)*(RAND_MAX)))                          //Exports element from train to test 20% of the times
		{
			(*test)=insertAtEnd((*test),curr->f,nattr);        //Inserting element to test
			(curr->next)->prev = curr->prev;                   //DEleting element from train
            (curr->prev)->next = curr->next;
            temp=curr;
            curr=curr->prev;
            free(temp);
            count++;
		}
	}
	return count;
}

//Function to predict the classification of a given test datapoint based on the train dataset 
int predict(dptr head,int k,int c)
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

//Function to make classifications based on the knn algorithm
int* knnclassifier(nptr test,nptr train,int nattr,int k)
{
    int* predictions=malloc(sizeof(int)*testsize);
    int c=3;                                                       //No. of classifications in the dataset
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
        predictions[i]=predict(distances,k,c);   //Predicting a class for the test dataset based on the first k elements of the linked list just created
        temp_test=temp_test->next;
        i++;
        
    }while(temp_test!=test);
    return(predictions);
}

int main(void)
{
	srand(time(0));      //to generate a random seed for the rand() function
    nptr train;          //Linked list that contains the train dataset(Entire dataset initially)
    train = NULL;
    char* filename = "pid.csv";    //File to scan data from
    int atr=5;                     //No. of attributes
    train = listify(filename, atr, train);   //Creating linked list of the given dataset
    nptr test; 
    test=NULL;
    testsize=splitdata(&train,&test,atr,0.8);  //Splitting data into train and test based on the required split
    int k;                                     //K is entered by the user
    printf("Enter K: ");
    scanf("%d",&k);
    float accuracy=0;
	int counter=0;
    for(int i=0;i<1000;i++)                   //Loop that runs the classifier 1000 times and calculates the mean accuracy
    {
        int* predictions=malloc(sizeof(int)*testsize);
        predictions=knnclassifier(test,train,atr,k);    //Calling knnclassifier to storing predictions in an array
        int correct=0;
        nptr temp=test;
        int i=0;
        do                                              //Comparing predictions for test dataset with the actual classes of the test datapoints
        {
            if((int)temp->f[atr-1]==predictions[i])
                correct+=1;
            i++;
            temp=temp->next;
        }while(temp!=test);
        accuracy+=((float)correct)/testsize;
        counter++;
        printf("\rPercentage Completed: %d%%",(counter/10));   //Printing percentage of operation completed
    }
    printf("\nAccuracy= %g%%\n",(accuracy/10));               //Printing accuracy
}
