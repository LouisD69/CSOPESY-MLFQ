# CSOPESY-MLFQ

In a scenario where at the same time:

1. Topmost queue is currently empty
2. A process in lower queue is running in CPU
3. a new process arrives
4. a priority boost should occur

Behavior: The arriving process will be enqueued first before priority boosting

