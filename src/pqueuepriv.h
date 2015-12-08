/*
 * File: pqueuepriv.h (linked list version)
 * ----------------------------------------
 * This file contains the private section of the list-based PriorityQueue class.
 */

/*
 * Implementation notes: PriorityQueue data structure
 * --------------------------------------------------
 * The link-based priority queue uses a linked list to store the elements
 * of the queue. This implementation mantains a pointer to a dummy cell,
 * which is positioned at the start of the priority queue, and allocated
 * in the heap when the queue is initialized. The purpose of the dummy cell
 * is to remove the need to implement and maintain special-case code when
 * adding new elements to the queue. Therefore, the value and priority fields
 * of the dummy cell do not store any values.
 *
 * The following diagram illustrates the structure of a queue containing a
 * dummy cell, and two elements, A, and B, with priority values 1, and 2,
 * respectively:
 *
 *
 *       +-------+        +-------+         +-------+         +-------+
 *  head |   o---+------> |   /   |   +---> |   A   |   +---> |   B   |
 *       +-------+        +-------+   |     +-------+   |     +-------+
 *                        |   /   |   |     |   1   |   |     |   2   |
 *                        +-------+   |     +-------+   |     +-------+
 *                        |   o---+---+     |   o---+---+     |  NULL |
 *                        +-------+         +-------+         +-------+
 *
 *
 */

private:

/* Type used for each heap entry */

    struct Cell {
       ValueType value;
       double priority;
       Cell *link;
    };

/* Instance variables */

    Cell *head;      // Pointer to the dummy cell at the head
    int count;       // Number of elements in the queue

/* Private method prototypes */

    void deepCopy(const PriorityQueue & src);
