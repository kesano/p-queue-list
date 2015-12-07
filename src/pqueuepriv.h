/*
 * File: pqueuepriv.h (linked list version)
 * ----------------------------------------
 * This file contains the private section of the list-based PriorityQueue class.
 */

/*
 * Implementation notes: PriorityQueue data structure
 * --------------------------------------------------
 * The link-based priority queue uses a linked list to store the elements
 * of the queue. To ensure that adding a new element to the end of the
 * queue is fast, the data structure maintains a pointer to the last cell
 * in the queue as well as the first. If the queue is empty, both the
 * head pointer and tail pointer are set to NULL.
 *
 * The following diagram illustrates the structure of a queue containing
 * three elements, A, B, and C, with priority values 1, 2, and 3, resprectively:
 *
 *
 *       +-------+        +-------+         +-------+          +-------+
 *  head |   o---+------> |   A   |   +---> |   B   |   +--==> |   C   |
 *       +-------+        +-------+   |     +-------+   |  |   +-------+
 *  tail |   o---+---+    |   1   |   |     |   2   |   |  |   |   3   |
 *       +-------+   |    +-------+   |     +-------+   |  |   +-------+
 *                   |    |   o---+---+     |   o---+---+  |   |  NULL |
 *                   |    +-------+         +-------+      |   +-------+
 *                   |                                     |
 *                   +-------------------------------------+
 *
 *
 */

private:

/* Type used for each heap entry */

    struct Cell {
       std::string value;
       double priority;
       Cell *link;
    };

/* Instance variables */

    Cell *head;      // Pointer to the cell at the head
    Cell *tail;      // Pointer to the cell at the tail
    int count;       // Number of elements in the queue

/* Private method prototypes */

    void deepCopy(const PriorityQueue & src);
    bool isNextPriority(Cell *cp, Cell *pair);
    bool isHighestPriority(Cell *cp, Cell *pair);
    void appendToEnd(Cell *cp, Cell *pair);
