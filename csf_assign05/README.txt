Sample README.txt

Synchronization take place in the server, room, and the messagequeue's constructor.
This is because we want to use a queue style approach for synchronization. While there may
be multiple threads, each one has only one queue. The idea is that the workers must wait until
there is actually something in the queue before taking something out. As we went over in class,
enqueue will call P on the semaphore, and dequeue waits until there is something in the queue before
taking anything out. 

Since the list of members in room.h is something that can change, but is a resource needed by all threads,
we guard before doing anything to members throughout our code. 

In the server.cpp, the function called find_or_create_room would required to
be guarded. Since there will be multiple rooms inside the server, it is important
to guard initalizing the room in order to keep each room seperate from each other.

In room.cpp, guard must be called for add_member, remove_member, and broadcast message.
Add and remove member must make sure a member is not being added in multiple rooms 
after one call. Broadcast message must be guarded since we are mutating a member's 
messagequeue. These guards are accounting for mutations regarding the members
inside the rooms.

In messagequeue, the code is constantly altering the messages inside the queue.
Therefore a guard must be called for enqueue and dequeue. While enqueue is simple
since the function calls push_back, dequeue must wait for the sem_timedwait
before guarding to prevent bugs.


-Nathan and Michael Kim