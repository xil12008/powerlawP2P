import decimal as dc
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.legend_handler import HandlerLine2D
from scipy import optimize
import numpy as np
import random

dc.setcontext(dc.Context(prec=32))
context = dc.getcontext()

k = 2 
m = 10 
gamma = dc.Decimal(2.5)
b = 2*k

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

#Minimize: c^T * x
#Subject to: A_ub * x <= b_ub
#            A_eq * x == b_eq

negI =  np.negative( np.identity(m-k) )
I =   np.identity(m-k) 
ZERO = np.zeros(( m-k, m-k))

A = np.concatenate( (negI, negI), axis = 1)
B = np.concatenate( (negI, I), axis = 1)
C = np.concatenate( (ZERO, negI), axis = 1)
D = np.concatenate( (ZERO, negI), axis = 1)

ONE = np.ones(m-k)
A_UB = np.concatenate(
         ( np.concatenate(
             (np.concatenate(( A, B), axis = 0), C), axis = 0
           )
           , D
         ), axis = 0 )
print A_UB 

CB = np.zeros((m-k,1))
for i in range(k, m, 1): 
    CB[i-k,0] = sumf[i] 
for i in range(b, m, 1):
    CB[i-k,0] -= 1 

#random s
s = np.zeros((m-k,1))
for i in range(b):
    s[random.randint(0, m-k-1),0] += 1
negs = np.negative(s)

B_UB = np.concatenate(
         ( np.concatenate(
             (np.concatenate(( negs, s), axis = 0), np.zeros((m-k,1))), axis = 0
           )
           , CB 
         ), axis = 0 )
print B_UB.transpose()

#Minimize: c^T * x
#Subject to: A_ub * x <= b_ub
#            A_eq * x == b_eq
#By default bounds are (0, None) (non-negative)

C = np.ones((1, 2*(m-k))).tolist()[0], 
print len(C[0])
print A_UB.tolist()

print optimize.linprog(
    c = C[0], 
    A_ub = A_UB.tolist(), #[[1, 1]], 
    b_ub = B_UB.tolist(), #[6],
    method='simplex'
)
