
// threadtest.cc 
//	Test cases for the threads assignment.

#include "copyright.h"
#include "system.h"
#include "synch.h"

// testnum is set in main.cc
int testnum = 3;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// LockTest1
//----------------------------------------------------------------------

Lock *locktest1 = NULL;

void
LockThread1(int param)
{
    printf("L1:0\n");
    locktest1->Acquire();
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    locktest1->Release();
    printf("L1:3\n");
}

void
LockThread2(int param)
{
    printf("L2:0\n");
    locktest1->Acquire();
    printf("L2:1\n");
    currentThread->Yield();
    printf("L2:2\n");
    locktest1->Release();
    printf("L2:3\n");
}

void
LockTest1()
{
    DEBUG('t', "Entering LockTest1");

    locktest1 = new Lock("LockTest1");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("two");
    t->Fork(LockThread2, 0);
}

// acquiring the same lock twice
void testLockTwice()
{  
    printf("Entering LockTest2\n");
    locktest1 = new Lock("LockTest2");

    locktest1->Acquire();
    printf("acquire clock time: 1\n");
    locktest1->Acquire();
    printf("acquire clock time: 2\n");
    printf("leaving locktest2\n");
}

// // releasing a lock that isn't held
// Lock *emptyLockTests = NULL;
// void ReleaseEmptyLock() {
// 	emptyLockTests = new Lock("empty()");
// 	emptyLockTests->Release();
// 	printf("releasing a lock that has not held.s\n");
// }

// // deleting a lock that is held
// Lock *emptyLockTests = NULL;
// void testDeleteEmptyLock() {
//     deleteLock = new Lock("empty()");
//     deleteLock->Acquire();
//     delete deleteLock;
//     printf("releasing a lock that has not held.s\n");
// }

// //waiting on a condition variable without holding a lock
// void testCondition() {
//     Condition *testCondition;
//     testCondition.wait();
//     printf("waiting on the.....\n");
// }

/* Signaling a condition variable wakes only one thread 
   and broadcasting wakes up all threads */
// void testSignalandBroadcast() {
//     Condition *testCondition;
//     Lock1 = new Lock("1");
//     Lock2 = new Lock("2");
//     Lock3 = new Lock("3");
//     Condition.wait(Lock1);
//     Condition.wait(Lock2);
//     Condition.wait(Lock3);
//     Condition.signal();
// }
// signaling and broadcasting to a condition variable with no waiters is a no-op, 
// and future threads that wait will block (i.e., the signal/broadcast is "lost")
// void testNoOp() {

// }
// /*a thread calling signal holds the lock passed in to signal*/
// void testSignalHoldLock() {

// }
// /* deleting a lock or condition variable should have no threads on the wait queue*/
// void testDeleteLockCVNoThreads() {

// }

//------------------------------------------------------------------------
//MailboxTest
//
//------------------------------------------------------------------------
// Mailbox *Mailboxtest = NULL;

// void MailboxTest() {
   
//     DEBUG('t', "Entering Mailboxtest");

//     Mailboxtest = new Mailbox();
//     Mailboxtest->Send(1);
//     int in = Mailboxtest->Receive();
//     printf("what is in the mailbox : %d \n ", in);

//     delete Mailboxtest;


// }



//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//---------------------------------------------------------------------
void
ThreadTest()
{
    switch (testnum) {
   // case 1:  ThreadTest1();  break;
    //case 2:  LockTest2();    break;
    //case 3:  LockTest3();    break;
    case 4:   testLockTwice(); break;
   // case 4:  ReleaseEmptyLock(); break;
  //  case 5:  testCondition(); break;
    default: 
	printf("No test specified.\n");
	break;
    }
}







