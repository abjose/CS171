
import bisect
import numpy as np

# implement spline stuff to test out


def B_0(u): return (1-u)**3 / 6
def B_1(u): return (3*u**3 - 6*u**2 + 4) / 6
def B_2(u): return (-3*u**3 + 3*u**2 + 3*u + 1) / 6
def B_3(u): return u**3 / 6
def B_i(i, u):
    if i == 0:   return B_0(u)
    elif i == 1: return B_1(u)
    elif i == 2: return B_2(u)
    elif i == 3: return B_3(u)


# t should be a nondecerasing sequence of knots...
# there should be n+k+1 knots
# should have k-1 0s and 1s at beginning and end of knot vector
# do binary search for knot
# pretty sure knots are in u, i.e. between 0 and 1...

# should somehow define 0/0 == 0

# knot vector
t = [0.,0.,0.,0.,1.,1.,1.,1.]
# control points - x,y,w
# should make control pt typdefs...
p = np.array([[-1., 1., 1.],
              [-1.,-1., 1.],
              [ 1.,-1., 1.],
              [ 1., 1., 1.]])

def N(i, k, u):
    # do with dynamic programming instead
    if k==1:
        if t[i] <= u <= t[i+1]: return 1
        return 0
    l,r = 0,0
    try:
        l = (u-t[i])*N(i,k-1,u) / (t[i+k-1] - t[i])
    except ZeroDivisionError:
        l = 0
    try:
        r = (t[i+k]-u)*N(i+1,k-1,u) / (t[i+k] - t[i+1])
    except ZeroDivisionError:
        r = 0
    return l+r


def Q(u, k):
    # NOTE: where is U vs u = U-i? If things don't look right should verify...
    sum = np.array([0.,0.,0.])
    for i in range(k): 
        sum += p[i] * N(i, k, u)
    return sum

def a(i,k,j, t_new):
    # j should be the index of the knot just to the left of the new one
    if 1 <= i <= j-k: return 1
    if j+1 <= i <= n: return 0
    if j-k+1 <= i <= j:
        return (t_new - t[i]) / (t[i+k] - t[i])
    
def insert_knot(t_new, k):
    # insert s.t. t_prev < t_new <= t_next
    # find indices of surrounding knots
    t_next = bisect.bisect_left(t, t_new)
    if t_next == len(t): raise ValueError('Uh oh, nothing found')
    if t_next == 0:      raise ValueError('Uh oh, found 0th element')
    t_prev = t_next-1;

    # insert knot...sure this is supposed to happen before getting a's?
    np.insert(t, t_next, t_new)

    # find new control points
    # insert new control point in relevant range
    j = t_prev
    np.insert(p, j-k+1, np.array([0.,0.,0.])) # value shouldn't matter
    for i in range(j-k+1,j+1):
        a_i  = a(i,k,t_prev, t_new)
        p[i] = (1-a_i)*p[i-1] + a_i*p[i]

if __name__=='__main__':
    print 'meow'

    # probably make something to 'sample' Q for lots of points...
    degree = 3
    k = degree + 1

    #for i in np.arange(0.,1.,0.01):
    #    print Q(float(i),k)

    insert_knot(0.4,k)
