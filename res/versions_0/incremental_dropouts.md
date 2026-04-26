# INCREMENTAL DROPOUTS

This isn't incremental in a stable sense, the chance of the event occuring is just getting higher.

`if random < progress ^ 2 then ...`

The dropouts themselves act more as a bitcrusher, as they're not taking in account the sample rate or byte rate or the bit depth. They simple switch singular bits off.

`... data = 0`