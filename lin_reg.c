#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct node *nptr;

typedef struct node{
    nptr next;
    nptr prev;
    float* f;
} dll;

int splitdata(nptr* train, nptr* test, int nattr);
nptr listify(char* filename, int nattr, nptr head);
nptr insertAtEnd(nptr head, float *f, int nattr);
void display(nptr head, int nattr);

float mean(nptr head, int i, int n);
float SS(float avg, nptr head, int i, float* deviation); //sum_of_squares
float SP(float* dev1, float* dev2, int n); //sum_of_products

float MSE(nptr test, float a, float b);

int total_count, test_count, train_count;

int main(void)
{
  srand(time(NULL));
  float *dx, *dy;
  float mean_x, mean_y, ss_x, ss_y, sp, b, a, mean_squared_error;
  int i, n;

  nptr train, test;
  train = NULL;
  test = NULL;
  char* filename = "cleanIris.csv";
  train = listify(filename, 5, train);
  n = splitdata(&train, &test, 5);
  display(train, 2);
  printf("%d\n", n);

  mean_x = mean(train, 0, n); //2 argument is column
  mean_y = mean(train, 1, n);

  dx = malloc(n*sizeof(float));
  dy = malloc(n*sizeof(float));

  printf("Mean X: %lg\n", mean_x);
  printf("Mean Y: %lg\n", mean_y);

  ss_x = SS(mean_x, train, 0, dx);
  ss_y = SS(mean_y, train, 1, dy);
  sp = SP(dx, dy, n);

  // y = bX + a

  b = sp/ss_x;
  a = mean_y - b*mean_x;

  printf("y = %lgX-%lg\n", b, a);

  mean_squared_error = MSE(test, a, b);
  printf("\nMSE = %lg\n", mean_squared_error);

 //printf("(X-Mx)^2 = %lf\n", s_x);

  return 0;

}

float mean(nptr head, int i, int n)
{
  float sum = 0;
  nptr curr = head;

  do{
    sum+=curr->f[i];
    ++n;
    curr = curr->next;
  }while(curr != head);

  return (sum/n);
}

float SS(float avg, nptr head, int i, float* deviation)
{
  int j = 0;
  float sum_of_squares;
  sum_of_squares = 0;
  nptr curr = head;

   do{
      deviation[j] = curr->f[i] - avg;
      sum_of_squares+=pow(deviation[j], 2);
      ++j;
      curr = curr->next;
    }while(curr != head);

  return sum_of_squares;
}

float SP(float* dev1, float* dev2, int n)
{
  int i;
  float sum_of_products;
  sum_of_products = 0;
  for(int i = 0; i < n; ++i)
    sum_of_products+= (dev1[i]*dev2[i]);

  printf("Sum of Products: %lg\n", sum_of_products);
  return sum_of_products;
}

int splitdata(nptr* train, nptr* test, int nattr)
{
    nptr curr,temp;

    for(curr=(*train)->next;curr!=(*train);curr=curr->next)
    {
        if(rand()>((0.8)*(RAND_MAX)))
        {
            (*test)=insertAtEnd((*test),curr->f,nattr);
            (curr->next)->prev = curr->prev;
            (curr->prev)->next = curr->next;
            temp=curr;
            curr=curr->prev;
            free(temp);
            ++test_count;
        }
      ++total_count;
    }
    train_count = total_count - test_count;
    return train_count;
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

    else
    {
      nptr curr = head;

        do
        {
          for(int i=0; i<nattr; i++) printf("%g, ", curr->f[i]);
          printf("\n");
          curr = curr->next;
        }while(curr != head);
    }
}

float MSE(nptr test, float a, float b)
{
  nptr curr;
  curr = test;
  float predicted, diff, MSE;
  MSE = 0;

  do{
  predicted = b*curr->f[0] + a;
  diff = curr->f[1] - predicted;
  MSE+=pow(diff, 2);
  curr = curr->next;
  }while(curr!=test);

  MSE = MSE/test_count;

  return MSE;
}
