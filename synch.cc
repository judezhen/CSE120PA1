// synch.cc
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    while (value == 0) { 			// semaphore not available
        queue->Append((void *)currentThread);	// so go to sleep
        currentThread->Sleep();
    }
    value--; 					// semaphore available,
    // consume its value

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void 
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
        scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}


// Dummy functions -- so we can compile our later assignments
// Note -- without a correct implementation of Condition::Wait(),
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
    name = debugName;
    queue = new List;

}

Lock::~Lock() {
    delete queue;
}

bool Lock::isHeldByCurrentThread() {
    return lockOnwer == currentThread;
}

void Lock::Acquire(){ 

    ASSERT(isHeldByCurrentThread()==true);

    IntStatus oldLevel = interrupt->SetLevel(IntOff);

     while ( lockValue ) {                      // lock is free
        queue->Append((void *)currentThread);       // so go to sleep
        currentThread->Sleep();
    }
    lockOnwer = currentThread;
    lockValue = true;

    (void) interrupt->SetLevel(oldLevel);
    
}


void Lock::Release() {
    Thread *thread; 
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
   
    //ASSERT(conditionLock->isHeldByCurrentThread());
    	
    thread = (Thread *)queue->Remove();
    
    if (thread != NULL)    // make thread ready, consuming the V immediately
        scheduler->ReadyToRun(thread);
    lockOnwer = NULL;
    lockValue = false;
    (void) interrupt->SetLevel(oldLevel);

}



Condition::Condition(char* debugName) {
    // name = debugName;
    // waitingQueue = new List;
 }
Condition::~Condition() { 
    delete waitingQueue;
}

void Condition::Wait(Lock* conditionLock) {
    // IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // ASSERT(conditionLock->isHeldByCurrentThread());
    // conditionLock->Release(); 
    // waitingQueue->Append((void *)currentThread); 
    // currentThread->Sleep();
    // conditionLock->Acquire();
    // (void) interrupt->SetLevel(oldLevel);
}

void Condition::Signal(Lock* conditionLock) { 
    
    // Thread *thread;
    // ASSERT(conditionLock->isHeldByCurrentThread());
   
    // IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // thread = (Thread *)waitingQueue->Remove();
    
    // if(thread != NULL) 
    //     scheduler->ReadyToRun(thread);
    // (void) interrupt->SetLevel(oldLevel);
}

void Condition::Broadcast(Lock* conditionLock) { 

    // Thread *thread;
    // ASSERT(conditionLock->isHeldByCurrentThread());
    // IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // thread = (Thread *)waitingQueue->Remove();
    // // wake up all the threads
    // while(thread != NULL) {
    //     scheduler->ReadyToRun(thread);
    //     thread = (Thread *)waitingQueue->Remove();
    // }
    // (void) interrupt->SetLevel(oldLevel);

}

// MailBox for part 2
// not working
// constructors
// Mailbox::Mailbox(char* debugName){   

//     name = debugName;
//     meet = Condition();
//     Sender = new  Condition();
//     receive = new Condition();
//     lock = new Lock();
//     msg = new List<>;

//     // receiver = new Semaphore("receiver", 0);
//     // sender = new Semaphore("sender", 0);
//     // same = new Semaphore("same", 0);
//     // lock = new Lock("lock");
//     // msg = new List();
// }

// Mailbox::~Mailbox(){
//     // delete receiver;
//     // delete sender;
//     // delete same;
//     // delete lock;
//     // delete msg;
    
// }

// void Mailbox::Send(int message){   
//   // // if the receiver receives a message, wake up  sender;
//   // sender->V();
//   // //receiver wait until the sender sends a msg;
//   // receiver->P();

//   // lock->Acquire();
//   // msg->Append((int *)message);
//   // lock->Release();

//   // same->V();

//   // sender->V();
//   // receiver->P();

// }

// void Mailbox::Receive(int *message){
    
//     // receiver->V();
//     // sender->P();
//     // same->P();

//     // lock->Acquire();
//     // message = (int * )msg->Remove();
//     // lock->Release();

//     // receiver->V();
//     // sender->P();

// }
























