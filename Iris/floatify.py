import csv
import math

f = open('iris.csv','rU')
fout = open('cleanIris.csv', 'w')
reader = csv.reader(f, delimiter=',')
for row in reader:
    outrow = ''
    for col in row:
        if col == "Iris-virginica":
            outrow = outrow+'2\n'
        elif col == "Iris-versicolor":
            outrow = outrow+'1\n'
        elif col == "Iris-setosa":
            outrow = outrow+'0\n'
        else:
            outrow = outrow + col + ','
    fout.write(outrow)    