
// threadtest.cc 
//  Test cases for the threads assignment.

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
// BEGIN PART 1 TESTING CODE
//----------------------------------------------------------------------

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
void testDeleteEmptyLock() {
    Lock *lock;
    lock = new Lock("test3");
    lock->Acquire();
    delete lock;
}


//waiting on a condition variable without holding a lock
void testCondition() {
    Condition *testCondition = new Condition("ctest");
    Lock *lock = new Lock("ltest");
    testCondition->Wait(lock);
    printf("Test failed.\n");
}


/* Signaling a condition variable wakes only one thread 
   and broadcasting wakes up all threads 
   need to comment on what the successful results look like
   to prep for writeup
*/

//SIGNAL TEST ENVIRONMENT WITH TWO THREADS
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
    t = new Thread("two"); 
    t->Fork(testFunction2, 0);
}


//BROADCAST TEST ENVIRONMENT WITH THREE THREADS
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


/*  Signaling and broadcasting to a condition variable with 
    no waiters is a no-op, and future threads that wait will block 
    (i.e., the signal/broadcast is "lost")
    
    Explanation of success:
    Signal and Broadcast methods don't do anything because of how 
    they're written forks a new thread, acquires a lock, then waits. 
    Thread is blocked successfully when the print statement after
    it waits doesn't run.
 */

Lock *testLock = NULL;
Condition *testCond = NULL;

void testWait(int param) {
    testLock->Acquire();
    testCond->Wait(testLock);
    printf("Block unsuccessful.");
    //test that thread "one" is in the queue
}
void testNoOp() {
    testCond = new Condition("testNoOpCV");
    testLock = new Lock ("testNoOpLock");
    testCond->Signal(testLock); //check that nothing happened
    printf("Signal did nothing.\n");
    testCond->Broadcast(testLock); //check that nothing happened
    printf("Broadcast did nothing.\n");
    Thread *t = new Thread("one");
    t->Fork(testWait,0);
}

/*  Fail when a thread calling signal holds the lock passed in to signal
    Will make an assertion fail in Signal because Signal checks if
    the calling thread holds the lock being passed in.
*/
void testSignalHoldLockWait(int param) {
    printf("Acquiring\n");
    testLock->Acquire();
    printf("Signaling\n");
    testCond->Signal(testLock);
}
void testSignalHoldLock() {
    testCond = new Condition("testSignalHoldLockCV");
    testLock = new Lock ("testSignalHoldLockLock");
    Thread *t = new Thread("one");
    t->Fork(testSignalHoldLockWait,0);
}


/*  deleting a lock or condition variable should have no threads on 
    the wait queue
*/

void testDeleteLockCVNoThreads() {}

//----------------------------------------------------------------------
// END PART 1 TESTING CODE
//----------------------------------------------------------------------


//------------------------------------------------------------------------
// BEGIN PART 2 TESTING CODE
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

//------------------------------------------------------------------------
// END PART 2 TESTING CODE
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// BEGIN PART 3 TESTING CODE
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// END PART 3 TESTING CODE
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// BEGIN PART 4 TESTING CODE
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// END PART 4 TESTING CODE
//------------------------------------------------------------------------


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//---------------------------------------------------------------------
void
ThreadTest()
{
    switch (testnum) {
    case 1: testLockTwice(); break;
    case 2: testReleaseUnheldLock(); break;
    case 3: testCondition(); break;
    case 4: testSignal(); break;
    case 5: testBroadcast(); break;
    case 6: testNoOp(); break;
    case 7: testSignalHoldLock(); break;
    default: 
	printf("No test specified.\n");
	break;
    }
}