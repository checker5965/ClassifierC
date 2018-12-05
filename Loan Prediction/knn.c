#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

int testsize;

typedef struct node *nptr;

//struct
typedef struct node{
    struct node* next;
    struct node* prev;
    float* f;
} dll;

typedef struct dnode *dptr;

//struct
typedef struct dnode{
    struct dnode* next;
    struct dnode* prev;
    int f;
    float d;
} dnn;

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

dptr insertinorder(dptr head,float f,int nattr, float d)
{
    dptr curr = head;
    dptr p = malloc(sizeof(dnn));
    p->d = d;
    p->f = (int)f;
    if(curr==NULL)
    {
        head = p;
        p->next = p;
        p->prev = p;
        return(head);
    }
    else if(d<=curr->d)
    {
        p->next = head;
        p->prev = head->prev;
        (head->prev)->next=p;
        head->prev=p;
        return(p);
    }
    else
    {
        for(curr=head->next;curr!=head;curr=curr->next)
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
        p->next = head;
        p->prev = head->prev;
        (head->prev)->next = p;
        head->prev = p;
        return(head);
    }
}

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

int splitdata(nptr* train, nptr* test, int nattr,float split)             //Need to create test linked list first and initialize to null
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

int predict(dptr head,int k,int c)
{
    int cl[c];
    for(int i=0;i<c;i++)
        cl[i]=0;
    
    for(int i=0;i<k;i++)
    {
        cl[head->f]+=1;
        head=head->next;
    }
    int max=0;
    for(int i=1;i<c;i++)
    {
        if(cl[i]>cl[max])
        {
            max=i;
        }
        else if(cl[i]==cl[max])
        {
            if(rand()>((0.8)*(RAND_MAX)))
            {
                max=i;
            }
        }
    }
    return(max);

}

int* knnclassifier(nptr test,nptr train,int nattr,int k)
{
    int* predictions=malloc(sizeof(int)*testsize);
    int c=2;
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
            for(int j=0;j<nattr-1;j++)
                distance+=(fabs(temp_train->f[j]-temp_test->f[j])*(temp_train->f[j]+temp_test->f[j]));
            distances=insertinorder(distances,temp_train->f[nattr-1],nattr,sqrt(distance));
            temp_train=temp_train->next;
        }while(temp_train!=train);
        predictions[i]=predict(distances,k,c);
        temp_test=temp_test->next;
        i++;
        
    }while(temp_test!=test);
    return(predictions);
}

int main(void)
{
	srand(time(0));
    nptr train;
    train = NULL;
    char* filename = "cleanBanker.csv";
    int atr=21;
    train = listify(filename, atr, train);
    nptr test;
    test=NULL;
    testsize=splitdata(&train,&test,atr,0.8);
    int k;
    printf("Enter K: ");
    scanf("%d",&k);
    float accuracy=0;
	for(int i=0;i<1000;i++)
    {
        int* predictions=malloc(sizeof(int)*testsize);
        predictions=knnclassifier(test,train,atr,k);
        int correct=0;
        nptr temp=test;
        int i=0;
        do
        {
            if((int)temp->f[atr-1]==predictions[i])
                correct+=1;
            i++;
            temp=temp->next;
        }while(temp!=test);
        accuracy+=((float)correct)/testsize;
    }
    printf("Accuracy= %g%%\n",(accuracy/10));
}