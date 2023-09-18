#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {
    /* Your code here! */
    if (node->var < tol) {
        return true;
    } else {
        return false;
    }
}
