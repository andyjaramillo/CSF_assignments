Implementing synchronization in this assignment mainly made sure we idnetified ciritcal sections where shared data would be 
accessed by multiple threads and adding guards in case of that. 

First in the server flow comes find and create rooms. This is a critical section because if two threads try to call the function
at the same time, then two rooms with the same name could be created, and thats not possible since in our implementation we 
use a hashmap and so one room may be overwritten and lost in the process. We chose Guard since the lock should last through the process of checking and making the room, and guard will keep the mutex locked until the end of the function. There wont be case of
a deadlock because even if a room is not found, a room is created so each thread then will find or create a room.

Second was in both add and remove member within room.cpp. When adding a member, rooms are essentially a set of users. Since there
is one set per room, in the chance that two users want to join the same room at the same time, this would cause a race to who add 
themselves. This can lead to the set being in an inconcsistent state. We added a guard in each case since the function only has one
line so the guard should span the entire function before unlocking the mutex.

Third was in broadcast message. The critical section is that we are enqueing messages onto a user's queue. If two sender threads
are trying to add messages at the same time since receivers will be in a while loop waiting for the message to come in to deque, but
since it could be the queue they are adding it to, one message could be lost in the data race. We added a Guard at the top so one thread at a time will enqueue.

Fourth was in message queue in side the destructor. Since all the messages on the queue are dynamically allocated before enqueued, 
we need to free them, but we dont want two threads freeing the same list of messages at the same time since then one thread will 
want to free messages that do not exist in memory and most likely lead to a segmentation fault because of an invalid memory access.

Fifth was in the enqueue functon. Just like with broadcast, this is mainly focused on the exact same queue. In enqueue we enacting
the same prevention of two threads adding to the same queue at the same time.

Lastly is on the dequeue function. For thread synchronization purposes, we want to have some form of locking to make sure that
considering the time when messages come in, there is no way that two messages can be dequeued at the same time. The reason we 
used a mutex specifically here instead of a guard is that once they are popped off, then they can run in parallel since they are 
dynamically allocated as two different messages, if two were there as continuing with the example. We unlock the mutex as if
we are letting the threads with the newly popped message call connection->send.