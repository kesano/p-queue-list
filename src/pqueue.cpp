/*
 * File: pqueue.cpp (linked-list version)
 * --------------------------------------
 * This file implements the pqueue.h interface using a linked list as the
 * underlying representation.
 */

#ifdef _pqueue_h

#include <iostream>
#include <string>
#include "console.h"
#include "error.h"
#include "pqueue.h"
using namespace std;

/*
 * Implementation notes: Constructor
 * ---------------------------------
 * Initializes a priority queue object containing a dummy cell, and updates
 * its fields.
 */

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
    Cell *cp = new Cell;
    cp->link = NULL;
    head = cp;
    count = 0;
}

/*
 * Implementation notes: Destructor
 * --------------------------------
 * This method frees any heap memory associated with the priority queue by
 * traversing the linked list to delete each cell.
 */

template <typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
    clear();
}

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
    while (count > 0) {
        dequeue();
    }
}

/*
 * Implementation notes: enqueue
 * -----------------------------
 * This implementation is able to add an element before any cell in the
 * list due to the existence of the dummy cell. Each cycle of the loop
 * checks whether the priority of the new element is higher than the
 * priority of the next cell after the cell pair is currently pointing to.
 * The only special case this implementation has to check for is when the
 * link field of pair is null, which means that the end of the list has
 * been reached. Thus, the new element is added to the end of the list.
 */

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(ValueType value, double priority) {
    Cell *cp = new Cell;
    cp->value = value;
    cp->priority = priority;
    if (isEmpty()) {
        head->link = cp;
        cp->link = NULL;
    } else {
        for (Cell *pair = head; pair != NULL; pair = pair->link) {
            if (pair->link == NULL) {
                pair->link = cp;
                cp->link = NULL;
                break;
            }
            if (cp->priority < pair->link->priority) {
                cp->link = pair->link;
                pair->link = cp;
                break;
            }
        }
    }
    count++;
}

/*
 * Implementation notes: dequeue
 * -----------------------------
 * Returns the value of the element after the dummy cell. This implementation
 * copies the location of that element and stores it in a new pointer variable,
 * in order for that cell to be accessible and its heap memory freed once the
 * dummy cell before it has been updated to link to the rest of the list after
 * it.
 */

template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty priority queue");
    Cell *cp = head->link;
    ValueType result = cp->value;
    head->link = cp->link;
    delete cp;
    count--;
    return result;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (isEmpty()) error("peek: Attempting to peek an empty priority queue");
    return head->link->value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
    if (isEmpty()) error("peekPriority: Attempting to peek an empty priority queue");
    return head->link->priority;
}

/*
 * Implementation notes: copy constructor
 * --------------------------------------
 * Initializes the current object to be a deep copy of the argument.
 */

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue(const PriorityQueue & src) {
    deepCopy(src);
}

/*
 * Implementation notes: assignment operator
 * Usage: q1 = q2;
 * -----------------------------------------
 * Assigns q2 to q1 so that the two priority queues function as independent
 * copies. For the linked list implementation of the PriorityQueue class,
 * this kind of assignment requires making a deep copy that includes the
 * linked list.
 */

template <typename ValueType>
PriorityQueue<ValueType> & PriorityQueue<ValueType>::operator=(const PriorityQueue & src) {
    if (this != &src) {
      clear();
      deepCopy(src);
    }
    return *this;
}

/*
 * Implementation notes: deepCopy
 * ------------------------------
 * This method copies the data from the priority queue passed as a parameter
 * into the current object, by reproducing its internal linked-list state in
 * the form of a new linked list. In order for each copy of a cell to have
 * access to cells yet to be allocated in the heap, this implementation stores
 * the address of each current copied cell in a new pointer variable, and updates
 * the link pointer in that cell to point to the next cell, after that next cell
 * has been allocated in the heap.
 */

template <typename ValueType>
void PriorityQueue<ValueType>::deepCopy(const PriorityQueue & src) {
    Cell *prev;
    for (Cell *cp = src.head; cp != NULL; cp = cp->link) {
        Cell *copy = new Cell;
        if (cp == src.head) {
            head = copy;
        } else {
            copy->value = cp->value;
            copy->priority = cp->priority;
            prev->link = copy;
        }
        copy->link = NULL;
        prev = copy;
    }
    count = src.size();
}

#endif
