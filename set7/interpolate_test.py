"""
Hmm, should pre-gen all interpolated transforms? Or do it on-the-fly...
Probably more sensible to pre-gen.
"""

# implement normal cubic for now, try catmull-rom after


def interpolate(u, k0, k1):
    return k0*(2*u**3-3*u**2+1) + k1*(3*u**2-2*u**3)

def interpolate_2(u, k0, k1, k2, k3):
    # get better var names...
    # interpolates between k1 and k2 using catmull-rom
    # to use nicely, could just double first and last element, then call on 
    # in-between range
    # WHAT SHOULD dx BE??
    dx = 1.
    k1_p = 0.5*(k1-k0)/dx + 0.5*(k2-k1)/dx
    k2_p = 0.5*(k2-k1)/dx + 0.5*(k3-k2)/dx
    return k1*(2*u**3-3*u**2+1) + k2*(3*u**2-2*u**3) + \
           k1_p*(u**3-2*u**2+u) + k2_p*(u**3-u**2)


if __name__=='__main__':

    u  = 0;
    du = 0.1
    while u <= 1:
        print interpolate(u,0,1), "\t", interpolate_2(u,0,0,1,1);
        u += du
