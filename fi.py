import decimal as dc
import numpy as np
import matplotlib.pyplot as plt

dc.setcontext(dc.Context(prec=32))
context = dc.getcontext()

k = 1 
m = 10 
gamma = dc.Decimal(2.6)

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
    for i in range( k, m+1):
        #print i, ":", f[i]
        x.append(dc.Decimal(i).ln())
        y.append(dc.Decimal(f[i]).ln())
    plt.plot(x, y, 'ro')
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
