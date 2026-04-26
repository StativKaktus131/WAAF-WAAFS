# CHOPPED

Here, the sample is chopped apart and switched around. It takes a part of the sample and swaps it a bit to the right or to the left.

```
50 times:
    start = random(0.3)
    length = random(0.5)
    shift = random(0.3) - 0.15

    swap({start, length}, {buffer})
```