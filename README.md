# masp2catg
Attaches dimensions actually used for masp generated matrices.

# Usage
    # strip down masp result:
    ./masp2catg - < ...
    
    # add to original stream, different vector meaning:
    ./masp2catg + <input0.ppm> ... < ...
    
    # transient on meaning also returns same meaning:
    ./masp2catg t < ...

# Tips on what we actually do in this repository
Instead of implementing bitsofcotton/specific, we reduce and add extra dimensions by doing QR decompositions with some row exchanges.
We can make chain of \+ on this which we don't know how many output layer to stop but we don't have now.

