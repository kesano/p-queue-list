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
 * Initializes an empty priority queue.
 */

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
    head = tail = NULL;
    count = 0;
}

/*
 * Implementation notes: Destructor
 * --------------------------------
 * This method frees any heap memory associated with the priority queue by
 * traversing the linked list to delete each cell. In order for the rest
 * of the list to be accessible after a cell is deleted, this implementation
 * stores the pointer to the next cell in the list before deleting the cell.
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
    head = tail = NULL;
}

/*
 * Implementation notes: enqueue
 * -----------------------------
 * This implementation uses the following strategy to enqueue an element in a
 * list-based priority queue:
 *
 *   1. Dynamically allocate space for a new cell in the heap.
 *   2. Initialize the cell's fields to the specified value and priority.
 *  3a. If the list is empty, simply update head and tail to point to the new cell.
 *  3b. Otherwise, if there is at least one cell already in the list, traverse the
 *      list one cell at a time by first checking whether the new cell's priority
 *      is higher than the head cell.
 *        - If it is, make the new cell the head of the list.
 *        - Otherwise, if the new cell's priority is lower than the head cell:
 *          - If the list only contains a single cell, place the new cell as
 *            the last element in the list, and update tail to point to the
 *            new cell.
 *          - Otherwise, if the list contains more than one cell, from the head
 *            of the list, check whether the new cell has a higher priority than
 *            the second cell in the list.
 *              - If so, place the new cell immediately after head by updating
 *                head so that it points to the new cell. Then link the new cell
 *                to the rest of the list.
 *              - Otherwise, if the new cell is neither the highest priority nor
 *                the second highest priority, traverse the rest of the list to
 *                check if the new cell has a higher priority than the next cell.
 *                  - If so, place the new cell immediately before the next cell
 *                    in the list by pointing its link to the next cell and updating
 *                    the previous cell to point to the new cell.
 *                  - Otherwise, place the new cell at the tail of the queue after
 *                    traversing the entire list.
 */

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(ValueType value, double priority) {
    Cell *cp = new Cell;
    cp->value = value;
    cp->priority = priority;
    if (isEmpty()) {
        cp->link = NULL;
        head = tail = cp;
    } else {
        for (Cell *pair = head; pair != NULL; pair = pair->link) {
            if (pair != head) {
                if (pair->link != NULL) {
                    if (isNextPriority(cp, pair)) break;
                } else {
                    appendToEnd(cp, pair);
                    break;
                }
            } else {
                if (isHighestPriority(cp, pair)) break;
                if (count > 1) {
                    if (isNextPriority(cp, pair)) break;
                } else {
                    appendToEnd(cp, pair);
                    break;
                }
            }
        }
    }
    count++;
}

/*
 * Implementation notes: dequeue
 * -----------------------------
 * Returns the value of the element at the head of the list, removes the element
 * at the head of the list and updates head to point to the rest of the list.
 */

template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty priority queue");
    Cell *cp = head;
    ValueType result = cp->value;
    head = cp->link;
    delete cp;
    count--;
    return result;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (isEmpty()) error("peek: Attempting to peek an empty priority queue");
    return head->value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
    if (isEmpty()) error("peekPriority: Attempting to peek an empty priority queue");
    return head->priority;
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
 * This method copies the data from the priority queue passed as a parameter into
 * the current object, by reproducing its internal linked-list state in a new linked
 * list. In order for each copy of a cell to be able to link to cells yet to be
 * allocated in the heap, this implementation stores the address of each current
 * copied cell in the tail pointer, and updates the link pointer in that cell after
 * the next copied cell has been allocated in the heap, so that it can point to that
 * next cell.
 */

template <typename ValueType>
void PriorityQueue<ValueType>::deepCopy(const PriorityQueue & src) {
    head = tail = NULL;
    count = 0;
    if (!src.isEmpty()) {
        for (Cell *cp = src.head; cp != NULL; cp = cp->link) {
            Cell *copy = new Cell;
            copy->value = cp->value;
            copy->priority = cp->priority;
            copy->link = NULL;
            if (size() > 0) tail->link = copy;
            else head = copy;
            tail = copy;
            count++;
        }
    }
}

/*
 * Implementation notes: isNextPriority
 * Usage: if (isNextPriority(&cp, &pair)) ...
 * ------------------------------------------
 * Returns true and places the enqueued element, cp, immediately after the
 * specified pair in the queue if cp's priority value is greater than pair's but
 * less than the priority value of the element that originally followed pair.
 */

template <typename ValueType>
bool PriorityQueue<ValueType>::isNextPriority(Cell *cp, Cell *pair) {
    if (cp->priority < pair->link->priority) {
        cp->link = pair->link;
        pair->link = cp;
        return true;
    }
    return false;
}

/*
 * Implementation notes: isHighestPriority
 * Usage: if (isHighestPriority(&cp, &pair)) ...
 * ---------------------------------------------
 * Returns true and places the enqueued element, cp, at the head of the queue
 * if it contains the highest priority value.
 */

template <typename ValueType>
bool PriorityQueue<ValueType>::isHighestPriority(Cell *cp, Cell *pair) {
    if (cp->priority < pair->priority) {
        cp->link = pair;
        head = cp;
        return true;
    }
    return false;
}

/*
 * Implementation notes: appendToEnd
 * Usage: void appendToEnd(&cp, &pair);
 * ------------------------------------
 * Places cp as the last element in the queue.
 */

template <typename ValueType>
void PriorityQueue<ValueType>::appendToEnd(Cell *cp, Cell *pair) {
    pair->link = cp;
    cp->link = NULL;
    tail = cp;
}

#endif
