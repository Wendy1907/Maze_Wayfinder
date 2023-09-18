#include "qtcount.h"

bool qtcount::prunable(Node * node, int tol) const {
    return prunableHelper(node, tol, node->avg);
}

bool qtcount::prunableHelper(Node* node, int tol, RGBAPixel a) const {
    if (node == nullptr) {
        return false;
    } else if ((node->NW == nullptr) && (node->NE == nullptr) && (node->SW == nullptr) && (node->SE == nullptr)) {
       int color_dist = pow((node->avg.r - a.r), 2) + pow((node->avg.g - a.g), 2) + pow((node->avg.b - a.b), 2);
        return color_dist <= tol;
    } else {
        return (prunableHelper(node->NW, tol, a)) && (prunableHelper(node->NE, tol, a)) && (prunableHelper(node->SE, tol, a)) && (prunableHelper(node->SW, tol, a));
    } 
}