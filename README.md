# CSPC-526-Distributed-System
----
By Yuliang Li & Keyang Dong

# Lab 2 Work-split

Both contributed to:
- overall system design;
- test\* scripts that send request to check the system's behavior in multiple aspects: normal operations, durability and checkpoint, failure recovering and even when massive operations on load;
- Debugging and optimization.

## Yuliang Li

- Mainly took charge of updating disklog and maintaining in-memory log;
- Added design for models such as Agent, Disklog, Logblock.

## Keyang Dong

- Mainly took charge of updating checkpoint region on disk, serializing the in-memory graph, and deserializing it from the durable disk.
- Added design for models such as CheckPoint, Graph::checkpoint, Graph::restore.
