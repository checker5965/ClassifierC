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

int main(void)
{
    nptr head;
    head = NULL;
    char* filename = "test.csv";
    head = listify(filename, 3, head);
    display(head, 3);
}