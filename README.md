# masp2catg
Categorize virtual inputs from masp copied structures.

# Description
We can en-index using bitsofcotton/catg from raw inputs.
Analogy to this, we want to en-index via virtual masp copied structures.

So we input R^(4n), output R^(mn) for argv\[1\] == m groups.

# Tips on dependency
If we implement bitsofcotton/specific, this is equivalent to them with each bit as a categorizers.
Otherwise, we only need this for 4 group splitting but we cannot decide which vector is better to have with ours
(with QR decomposition, we have R^n, R^(n-1), R^(n-2), R^(n-3) vectors for some of the categorizes).

# Stub
This is a stub repository.
