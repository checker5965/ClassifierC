#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double mean(double* , int);
double SS(double avg, double* data, int n, double* deviation); //sum_of_squares
double SP(double* dev1, double* dev2, int n); //sum_of_products

int main(void)
{
  double *x, *y, *dx, *dy;
  double mean_x, mean_y, ss_x, ss_y, sp, b, a;
  int i, n;

  printf("Enter number of data points: ");
  scanf("%i", &n);

  x = malloc(n*sizeof(double));
  y = malloc(n*sizeof(double));
  dx = malloc(n*sizeof(double));
  dy = malloc(n*sizeof(double));

  printf("Enter x and y:\n");
  for(i = 0; i < n; ++i) scanf("%lg %lg", &x[i], &y[i]);

  mean_x = mean(x,n);
  mean_y = mean(y,n);

  printf("Mean X: %lg\n", mean_x);
  printf("Mean Y: %lg\n", mean_y);

  ss_x = SS(mean_x, x, n, dx);
  ss_y = SS(mean_y, y, n, dy);
  sp = SP(dx, dy, n);

  // y = bX + a

  b = sp/ss_x;
  a = mean_y - b*mean_x;

  printf("y = %lgX-%lg\n", b, a);

 //printf("(X-Mx)^2 = %lf\n", s_x);

  return 0;

}

double mean(double* a, int n)
{
  int i;
  double sum;
  sum = 0;
  for(i = 0; i < n; ++i) sum+=a[i];

  return (sum/n);
}

double SS(double avg, double* data, int n, double* deviation)
{
  int i;
  double sum_of_squares;
  sum_of_squares = 0;

  for(i = 0; i < n; ++i)
  {
    deviation[i] = data[i] - avg;
    sum_of_squares+=pow(deviation[i], 2);
  }

  return sum_of_squares;
}

double SP(double* dev1, double* dev2, int n)
{
  int i;
  double sum_of_products;
  sum_of_products = 0;
  for(int i = 0; i < n; ++i)
    sum_of_products+= (dev1[i]*dev2[i]);

  printf("Sum of Products: %lg\n", sum_of_products);
  return sum_of_products;
}
