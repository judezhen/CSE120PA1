
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
// part 1 tests
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
    Lock *lock;
    printf("Creating lock\n");
    lock = new Lock("test");

    lock->Acquire();
    printf("Acquired lock once\n");

    lock->Acquire();
    printf("Acquired lock twice\n");
}

// releasing a lock that isn't held
void testReleaseUnheldLock() {
    Lock *lock;
    lock = new Lock("test2");
    lock->Release();
	printf("Test failed.\n");
}

// deleting a lock that is held
// void testDeleteEmptyLock() {
//     Lock lock = new Lock("test");
//     lock->Acquire();
//     delete lock;
//     printf("\n");
// }

//waiting on a condition variable without holding a lock
void testCondition() {
    Condition *testCondition = new Condition("ctest");
    Lock *lock = new Lock("ltest");
    testCondition->Wait(lock);
    printf("Test failed.\n");
}

/*-----------------------------------------------------
               cv Sinal test
______________________________________________________ */


/* Signaling a condition variable wakes only one thread 
   and broadcasting wakes up all threads */
Condition *testCondtion_helper = new Condition("cond");
Lock * lock = new Lock("lock1");;

void testFunction1(int param){
        printf("testing Function 1 \n");
        lock->Acquire();
        testCondtion_helper->Wait(lock);
        printf("signaled Function 1\n");
        lock->Release();
}
void testFunction2(int param){
        testCondtion_helper->Signal(lock);
        printf("testing Function 2 \n");
        printf("signaled Function 2\n");
}



void testSignal() {
      
    Thread *t = new Thread("one");    
    t->Fork(testFunction1, 0);

    Thread *t2 = new Thread("two"); 
    t2->Fork(testFunction2, 0);


}


/*-----------------------------------------------------
               cv broadcast test
______________________________________________________ */


/* Signaling a condition variable wakes only one thread 
   and broadcasting wakes up all threads */
// Condition *testCondtion_helper = new Condition("cond");
// Lock * lock = new Lock("lock1");;

Condition *testCondtion_helper1 = new Condition("cond1");
Lock * lock1 = new Lock("lock1");;


void testFun1(int param){   
    lock1->Acquire();  
        printf("testing Function 1 \n");
        testCondtion_helper1->Wait(lock1);
        
        printf("broadcast Function 1\n");

}
void testFun2(int param){
    lock1->Acquire();  
         printf("testing Function 2 \n");
        testCondtion_helper1->Wait(lock1);
        
        printf("signaled Function 2\n");
  }

void testFun3 (int param){
        printf("testing Function 3\n");
        testCondtion_helper1->Broadcast(lock1);
     
        printf("signaled Function 3\n");       
}
void testBroadcast() {
    Thread *t = new Thread("one");  
    t->Fork(testFun1, 0);
    t = new Thread("two"); 
    t->Fork(testFun2, 0);
    t = new Thread("three"); 
    t->Fork(testFun3, 0);
}









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
    case 5:   testReleaseUnheldLock(); break;
    case 6:   testCondition(); break;
    case 7:   testSignal(); break;
    case 8:   testBroadcast(); break;
    default: 
	printf("No test specified.\n");
	break;
    }
}







