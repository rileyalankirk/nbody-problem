## Runtime
| Program  | random 25 | random 100 | random 250 | random 500 | random 1000 | random 10000 |
|----------|-----------|------------|------------|------------|-------------|--------------|
| nbody-s  | 0.006398  | 0.096148   | 0.593202   |  2.442646  |  9.655847   |  971.343806  |
| nbody-s3 | 0.006235  | 0.099653   | 0.630197   |  2.482704  |  10.282502  |              |
| nbody-p  | 0.048563  | 0.196989   | 0.246943   |  0.333839  |  0.497198   |  15.667279   |
| nbody-p3 | 0.059750  | 0.890496   | 6.886836   | 32.508843  | 176.910257  |              |

## Speedup & Efficiency
The regular parallel program gets significantly faster as the number of bodies increases.
This program is scalable.
Whereas the third law parallel program gets slower as the number of bodies increases.
This program is not scalable.

| Speedup    |   random 25   |  random 100   |   random 250   |   random 500   | random 1000    | random 10000 |
|------------|---------------|---------------|----------------|----------------|----------------|--------------|
|    s-p     | 7.6x slowdown | 2x slowdown   | 2.4x speedup   |  7.3x speedup  | 19.4x speedup  | 62x speedup  |
|   s3-p3    | 9.6x slowdown | 8.9x slowdown | 10.9x slowdown | 13.1x slowdown | 17.2x slowdown |              |


| Efficiency |   random 250  |   random 500  | random 1000    | random 10000   |
|------------|---------------|---------------|----------------|----------------|
|    s-p     | 59% parallel  | 87% parallel  |  96% parallel  |  99% parallel  |

The serial program is faster for small numbers of bodies for the regular parallel program. The serial program
is always faster than the third law parallel program.
The regular serial and parallel programs have a crossover point between 100 and 250 bodies.
The naïve program is faster than the third law program in the majority of cases we tested.