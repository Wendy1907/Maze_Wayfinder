
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/**
     * Constructor that builds a quadtree out of the given PNG.
     * Every leaf in the tree corresponds to a pixel in the PNG.
     * Every non-leaf node corresponds to a 2^k x 2^k square of pixels
     * in the original PNG, represented by an (x,y) pair for the
     * upper left corner of the square and an integer dimension, k.
     * In addition, the Node stores a pixel representing the average
     * color over the square. It also stores a measure of color
     * variability over the square.
     *
     * Every node's children correspond to a partition
     * of the node's square into four smaller squares. The
     * current node's square is split in half horizontally
     * and vertically to produce the 4 children.
     *
     * This function will also build the stats object used to compute
     * average pixel color and variability, over the squares.
     */
quadtree::quadtree(PNG& imIn) {
    /* Your code here! */
    int width = imIn.width();
    int height = imIn.height();

    stats s(imIn);
    int dim = floor(log2(min(width, height)));
    edge = pow(2, dim);

    root = buildTree(s, make_pair(0,0), dim);
}

 /* Private helper function for the constructor. Recursively builds
     * the tree according to the specification of the constructor.
     * @param s Contains the data used to calc avg and var
     * @param ul upper left point of current node's square.
     * @param dim reflects the size of the current square
     */
quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    /* Your code here! */
    if(dim == 0) {
        return new Node (ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
    } else {
        Node *newNode = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

        pair<int, int> ulNW = make_pair(ul.first, ul.second);
        pair<int, int> ulNE = make_pair(ul.first + ((pow(2, dim))/2), ul.second);
        pair<int, int> ulSE = make_pair(ul.first + ((pow(2, dim))/2), ul.second + ((pow(2, dim))/2));
        pair<int, int> ulSW = make_pair(ul.first, ul.second + ((pow(2, dim))/2));
        
        newNode->NW = buildTree(s, ulNW, (dim - 1));
        newNode->NE = buildTree(s, ulNE, (dim - 1));
        newNode->SE= buildTree(s, ulSE, (dim - 1));
        newNode->SW = buildTree(s, ulSW, (dim - 1));

        return newNode;
    }
}

/**
     * Render returns a PNG image consisting of the pixels
     * stored in the tree. It may be used on pruned trees. Draws
     * every leaf node's square onto a PNG canvas using the
     * average color stored in the node.
     */
PNG quadtree::render() const {
    /* Your code here! */
    PNG canvas(edge, edge);
    renderHelper(root, canvas);
    return canvas;
}

void quadtree::renderHelper(Node* node, PNG& image) const {
    if (node == nullptr) {
        return;
    } else if ((node->NW == nullptr) && (node->NE == nullptr) && (node->SW == nullptr) && (node->SE == nullptr)) {
        pair<int, int> ul = node->upLeft;
        int d = node->dim;
        int e = pow(2, d);

        int x = ul.first;
        int y = ul.second;

        for (int b = y; b < (y + e); b++) {
            for (int a = x; a <(x + e); a++) {
                RGBAPixel *pixel = image.getPixel(a, b);
                *pixel = node->avg;
            }
        }
    } else {
        renderHelper(node->NW, image);
        renderHelper(node->NE, image);
        renderHelper(node->SW, image);
        renderHelper(node->SE, image);
    }
}


/*
     * The idealPrune function can be considered to be the inverse
     * of the pruneSize function. It takes as input a number of leaves
     * and returns the minimum tolerance that would produce that resolution
     * upon a prune. It does not change the structure of the tree.
     */
int quadtree::idealPrune(const int leaves) const {
    int tol = 0;
    int size = pruneSize(tol);

    while ((size > leaves) || (size == 0)) {
        tol++;
        size = pruneSize(tol);
    }
    return tol;
}



/*
     * The pruneSize function takes a tolerance as input, and returns
     * the number of leaves that would result if the tree were to
     * be pruned with that tolerance. Consistent with the definition
     * of prune, a node is counted if prunable returns true.
     *
     * You may want a recursive helper function for this one.
     */
int quadtree::pruneSize(const int tol) const {
    /* Your code here! */

    return pruneSizeHelper(root, tol);
}

int quadtree::pruneSizeHelper(Node* node, const int tol) const{
    if (node == nullptr) return 0;
    else if (node->NW == nullptr) return 1;
    else if (prunable(node, tol)) return 1;
    int count = 0;
    count += pruneSizeHelper(node->NW, tol);
    count += pruneSizeHelper(node->NE, tol);
    count += pruneSizeHelper(node->SW, tol);
    count += pruneSizeHelper(node->SE, tol);
    return count;
}

/**
     *  Prune function trims subtrees as high as possible in the tree.
     *  A subtree is pruned (cleared) if prunable returns true.
     *  Pruning criteria should be evaluated on the original tree, not
     *  on a pruned subtree. (we only expect that trees would be pruned once.)
     *
     * You may want a recursive helper function for this one.
     */
void quadtree::prune(const int tol) {
    /* Your code here! */
    pruneHelper(root, tol);
}

void quadtree::pruneHelper(Node*& node, const int tol) {
    if (node == nullptr) {
        return;
    } else if (prunable(node, tol)) {
        clearHelper(node->NW);
        clearHelper(node->NE);
        clearHelper(node->SE);
        clearHelper(node->SW);

        node->NW = nullptr;
        node->NE = nullptr;
        node->SE = nullptr;
        node->SW = nullptr;
    } else {
        pruneHelper(node->NW, tol);
        pruneHelper(node->NE, tol);
        pruneHelper(node->SE, tol);
        pruneHelper(node->SW, tol);
    }
       
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current quadtree class.
     * You may want a recursive helper function for this one.
     */
void quadtree::clear() {
    /* your code here */
    clearHelper(root);
    root = NULL;
}

void quadtree::clearHelper(Node*& node) {
    if (node == nullptr) {
        return;
    } else {
        clearHelper(node->NW);
        clearHelper(node->NE);
        clearHelper(node->SW);
        clearHelper(node->SE);

        node->NW = nullptr;
        node->NE = nullptr;
        node->SW = nullptr;
        node->SE = nullptr;

        delete node;
        node = nullptr;
        
    }
}


/**
     * Copies the parameter other quadtree into the current quadtree.
     * Does not free any memory. Called by copy constructor and op=.
     * You may want a recursive helper function for this one.
     * @param other The quadtree to be copied.
     */
void quadtree::copy(const quadtree& orig) {
    /* your code here */
    root = copyHelper(orig.root);
    edge = orig.edge;
}

quadtree::Node* quadtree::copyHelper(Node* node) {
    if (node == nullptr) {
        return nullptr;
    } else {
        Node* copyNode = new Node(node->upLeft, node->dim, node->avg, node->var);
        copyNode->NW = copyHelper(node->NW);
        copyNode->NE = copyHelper(node->NE);
        copyNode->SE = copyHelper(node->SE);
        copyNode->SW = copyHelper(node->SW);

        return copyNode;
    }
}