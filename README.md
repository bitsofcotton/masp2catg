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

# What this repository actually do
However, we only do QR decomposition to reduce dimensions and pick up last one on each exchanges also loop them to the last dimension.
This is the reverse order differed to adding some of the specific values for the direction.

Either, this isn't guarantees the result is proper for now.

