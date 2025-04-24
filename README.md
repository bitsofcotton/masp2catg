# masp2catg
Attaches dimensions actually used for masp generated matrices.

# Usage
    ./masp2catg - < wgL.txt
    ./masp2catg + <input0.ppm> .. < wgL.txt

# Tips on what we actually do in this repository
Instead of implementing bitsofcotton/specific, we reduce and add extra dimensions by doing QR decompositions with some row exchanges.
We can make \+ chain of this but we don't have now.

