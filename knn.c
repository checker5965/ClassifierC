#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void splitdata(nptr* train, nptr* test, int nattr)
{
	nptr curr,temp;
	for(curr=(*train)->next;curr!=NULL;curr=curr->next)
	{
		if(rand()>((0.8)*(RAND_MAX)))
		{
			(*test)=insertAtEnd((*test),curr->data,nattr);
			temp=curr->prev;
			temp->next=curr->next;
			if(curr->next!=NULL)
			{
				temp=curr->next;
				temp->prev=curr->prev;
			}
			free(curr);
		}
	}
}

int main(void)
{
	FILE 
}