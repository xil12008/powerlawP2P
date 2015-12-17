import decimal as dc
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.legend_handler import HandlerLine2D

dc.setcontext(dc.Context(prec=32))
context = dc.getcontext()

k = 2 
m = 10 
gamma = dc.Decimal(2.5)

sum = dc.Decimal(0)
for j in range(k, m): 
    sum += (m-j) / context.power(j, gamma) 
print sum

f = {}

sumf = {} 
sumf[k-1] = 0
tmp = 0
for i in range( k, m):
    f[i] = (m - 2*k) / context.power(i, gamma) /  sum
    print "f[", i, "]=", f[i]
    sumf[i] = sumf[i-1] + f[i]
    tmp += i * f[i]
f[m] = 1 - sumf[m-1]
print "f[", m, "]=", f[m]
tmp += m * f[m]
sumf[m] = 1
print "average degree =", tmp


outfile = open("./ai.txt", "w+")
a = {}
a[k-1] = 1 
suma = 0
for i in range(k, m):
    a[i] = a[i-1] - f[i]
    suma += a[i]
    print "a[", i, "]=", a[i]
    outfile.write(str(a[i]) + "\n")
print "total neighbors #=", suma
print a[m-1], "=", f[m]

outfile.close()

def showfi():
    x = []
    y = []
    y_real = []
    y_dummy = []
    y_solution = []
    real =[ 0.473250,  0.174250,  0.087850,  0.047125,  0.031775,  0.020800,  0.016925,  0.008175,  0.139850 ]
    dummy = [ 0.740148,  0.198335,  0.017948,  0.013656,  0.012095,  0.010795,  0.004552,  0.001951,  0.000520 ] 
    solution = [ 0.473633,  0.172067,  0.091367,  0.046400,  0.031033,  0.020733,  0.018100,  0.003733,  0.142933 ]

    for i in range( k, m+1):
    #    #print i, ":", f[i]
    #    x.append(dc.Decimal(i).ln())
        x.append(i)
        y.append(f[i])
    #    y.append(dc.Decimal(f[i]).ln())
    #    y_real.append(dc.Decimal(real[i-k]).ln())
    #    y_dummy.append(dc.Decimal(dummy[i-k]).ln())
    #    y_solution.append(dc.Decimal(solution[i-k]).ln())

    plt.yscale('log')
    plt.xscale('log')

    line1, = plt.plot(x, y, marker='o', label='Theory')
    line2, = plt.plot(x, real, marker='o', label='Real')
    line3, = plt.plot(x, dummy, marker='o', label='Dummy Removal')
    line4, = plt.plot(x, solution, marker='o', label='Removal w/ approach')

    #pyplot.xscale('log')

    plt.legend(loc=0, handler_map={line1: HandlerLine2D(numpoints=2)})

    plt.show()


showfi()


'''
print "-"*15
for j in range(k, m):
    print "j=", j
    print "eu(k) = ", dc.Decimal(2.0) * (j - k) / (m - k + 1) 
    print "sumf[", j-1 ,"]=", sumf[j-1] 
    print "-"*15

j = 17 
eu = {}
eu[k] = dc.Decimal(2.0) * (j - k) / (m - k + 1) 
for key in range(k+1, m+1):
    if key - 1 < j:
        eu[key] = eu[k] - sumf[key-1] 
    else:
        eu[key] = eu[k] - sumf[key-1] + 1

eusum = 0
for key in range(k, m+1):
    eusum += eu[key]
    print "eu[", key, "]=", eu[key]
print eusum
'''
