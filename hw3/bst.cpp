#include "bst.hpp"

//#define HW_DEBUG

extern bool g_display_all;
extern bool g_is_avl;

Bst::Bst(istream &input) : _root(NULL), _input(input) {}

Bst::~Bst()
{
    clear(_root);
}

void Bst::doRun()
{
    if (FALSE == build()) {
        return;
    }

    display();
}

int Bst::build()
{
    char line[MAX_LINE+2];
    int num = 0;
    _input.getline(line, sizeof(line));
    while (!_input.eof()) 
    {
        num++;
        if (strlen(line) > MAX_LINE)
        {
            cerr << "line too long, No: " << num << endl;
            exit(1);
        }

        char *str = trim(line);
        Node *node = new Node(str);

        insert(node);
        if (g_display_all) {
            if (num != 1) {
                cout << endl;
            }
            cout << "Inserted \"" << str << "\" (node " << num << "):\n\n";
            display();
        }
        _input.getline(line, sizeof(line));
    }

    if (!g_display_all) {
        display();
    }
    return TRUE;
}

char *Bst::trim(char *str)
{
    if (str == NULL) {
        return NULL;
    }

    char *head = str;
    char *end = str + strlen(str)-1;
    while (head != NULL && 
        (*head == ' ' || *head == '\t' || *head == '\r'))
    {
        head++;
    }

    if (head == NULL) 
    {
        return NULL;
    }

    while (*end == ' ' || *end == '\t' || *end == '\r')
    {
        end--;
    }

    *(end+1) = '\0';

    return head;
}

int Bst::insert(Node *node)
{
    if (_root == NULL) 
    {
        _root = node;
        return TRUE;
    }

    Node *p = NULL;
    int res = find(node->_data, p);
    if (res == TRUE) {
        cerr << "conflict key " << node->_data << endl;
        exit(1);
    }
    //otherwise, p is the future parent of the node
    node->_parent = p;
    if (strcmp(p->_data, node->_data) > 0) {
        p->_left = node;
    } else {
        p->_right = node;
    }

    if (g_is_avl) {
        if (p->_balance != 0) {
            //to this point, no rotation needed
            //only update the balance, no update height
            p->_balance = 0;
        } else {
            checkAvl(node);
        }
    }
    return TRUE;
}

int Bst::getHeight(Node * node)
{
    if (NULL == node) 
        return 0;
    return node->_height;
}

void Bst::updateHeight(Node * node)
{
    if (NULL == node) {
        return;
    }

    int lh = getHeight(node->_left);
    int rh = getHeight(node->_right);

    node->_height = 1 + max(lh, rh);
    node->_balance = lh - rh;
}

void Bst::fixHeight(Node *node)
{
    updateHeight(node->_left);
    updateHeight(node->_right);
    updateHeight(node);
}

bool Bst::isBalance(Node * node)
{
    return (node->_balance <= 1) && (node->_balance >= -1);
}

Node *Bst::rotate(Node *node)
{
    if (node->_balance > 0) {
        if (node->_left->_balance >= 0) {
            return rotateLL(node);
        }  else if (node->_left->_balance < 0) {
            return rotateLR(node);
        }
    } else if (node->_balance < 0) {
        if (node->_right->_balance > 0) {
            return rotateRL(node);
        }  else if (node->_right->_balance <= 0) {
            return rotateRR(node);
        }
    }

    return node;
}


void Bst::checkAvl(Node * node)
{
    if (NULL == node) {
        return;
    }
    //from node, update it's ancestors' height, as well as check balance
    Node *pp = node->_parent;

    while (pp) {
        updateHeight(pp);

        if (!isBalance(pp)) {
            pp = rotate(pp);
        }

        pp = pp->_parent;
    }
}

int Bst::find(char *str, Node * &p)
{
    return itnFind(_root, str, p);
}

int Bst::itnFind(Node *root, char *str, Node * &p)
{
    if (root == NULL) {
        return FALSE;
    }

    if (strcmp(root->_data, str) == 0) {
        p = root;
        return TRUE;
    } else if (strcmp(root->_data, str) > 0) {
        if (root->_left != NULL) {
            return itnFind(root->_left, str, p);
        } else {
            p = root;
            return FALSE;
        }
    } else if (strcmp(root->_data, str) < 0) {
        if (root->_right != NULL) {
            return itnFind(root->_right, str, p);
        } else {
            p = root;
            return FALSE;
        }
    }

    return FALSE;
}

void Bst::display()
{
    uint64_t bits = 0;
    int level = 0;
    if (NULL == _root) {
        cout << "(EMPTY)" << endl;
        return;
    }
    _root->nodeDisplay(bits, level);
}

void Node::printPre(uint64_t bits, int level)
{
    if (level <= 0) {
        return;
    }

    cout << ' ';
    for (int i=1; i<level; i++) {
        if (bits & 1<<i) {
            //at this level, it is the right child
            cout << "     ";
        } else {
            cout << "|    ";
        }
    }
}

void Node::nodeDisplay(uint64_t bits, int level)
{
    printPre(bits, level);

    if (level != 0) {
        if (bits & 1<<level) {
            cout << "\\-- ";
        } else {
            cout << "+-- ";
        } 
    }

    cout << _data;
    if (g_is_avl) {
        char c = '\0';
        switch (_balance) {
            case 0:
                c = 'E';
                break;
            case 1:
                c = 'L';
                break;
            case -1:
                c = 'R';
                break;
        }
        cout << " (" << c << ")";
    }
#ifdef HW_DEBUG
    if (_parent) {
        cout << _parent->_data;
    }
#endif
    cout << endl;

    if (_left == NULL) {
        printPre(bits, level+1);
        cout << "+-- NULL\n";
    } else {
        _left->nodeDisplay(bits, level+1);
    }

    if (_right == NULL) {
        printPre(bits, level+1);
        cout << "\\-- NULL\n";
    } else {
        bits = bits | 1<<(level+1);
        _right->nodeDisplay(bits, level+1);
    }
}

void Bst::clear(Node *root)
{
    if (root != NULL) {
        clear(root->_left);
        clear(root->_right);
        delete root;
        root = NULL;
    }
}

void Bst::swap(Node *n1, Node *n2)
{
    char *str = n1->_data;
    n1->_data = n2->_data;
    n2->_data = str;
}

Node *Bst::getInOrder(Node *node)
{
    if (NULL == node) {
        return NULL;
    }

    Node *p = node->_right;
    while (p->_left) {
        p = p->_left;
    }

    return p;
}

int Bst::del(char *str)
{
    if (NULL == str) {
        cerr << "delete input null str\n";
        exit(1);
    }

    Node *node = NULL;
    if (FALSE == find(str, node)) {
        cerr << "can't find when delete: " << str << endl;
        exit(1);
    }

    if (strcmp(str, "D") == 0 ) {
        cout << "coming D\n";
    }

    if (node->_left != NULL && node->_right != NULL) {
        //find inorder successor
        Node *inorder = getInOrder(node);
        swap(node, inorder);

        if (inorder->_right)
        {
            inorder->_right->_parent = inorder->_parent;
        }
        if (inorder->_parent->_left == inorder) {
            inorder->_parent->_left = inorder->_right;
        } else {
            inorder->_parent->_right = inorder->_right;
        }

        if (g_is_avl) {
            checkAvl(inorder);
        }

        return TRUE;
        
    } else if (node->_left != NULL) {
        node->_left->_parent = node->_parent;
        if (node->_parent == NULL) {
            //root 
            _root = node->_left;
        } else {
            if (node->_parent->_left == node) {
                node->_parent->_left = node->_left;
            } else {
                node->_parent->_right = node->_left;
            }
        }
    } else if (node->_right != NULL){
        node->_right->_parent = node->_parent;
        if (node->_parent == NULL) {
            //root
            _root = node->_right;
        } else {
            if (node->_parent->_left == node) {
                node->_parent->_left = node->_right;
            } else {
                node->_parent->_right = node->_right;
            }
        }
    } else {
        if (node->_parent == NULL) {
            _root = NULL;
        } else {
            //both left and right are NULL
            if (node->_parent->_left == node) {
                node->_parent->_left = NULL;
            } else {
                node->_parent->_right = NULL;
            }
        }

    }

    if (g_is_avl) {
        checkAvl(node);
    }

    return TRUE;
}

/*
 * all in left child
 *         (A)
 *        /
 *       (B)    -->    (B)
 *      /             /   \
 *     (C)           (c)  (A)
 */
Node * Bst::rotateLL(Node * root)
{
    Node *lc = root->_left;
    //update root.parent.left
    if (root->_parent) {
        if (strcmp(root->_data, root->_parent->_data) > 0) {
            //root is the right child of its parent
            root->_parent->_right = lc;
        } else {
            root->_parent->_left = lc;
        }
    } else {
        _root = lc;
    }

    lc->_parent = root->_parent;
    root->_left = lc->_right;
    if (root->_left) {
        root->_left->_parent = root;
    }
    root->_parent = lc;
    lc->_right = root;

#ifdef HW_DEBUG
    cout << "after rotateLL \n";
    display();
#endif

    fixHeight(lc);
    return lc;
}

/*
 * all in right child
 *    (A)
 *      \
 *      (B)    -->    (B)
 *        \          /   \
 *        (C)       (A)  (C)
 */
Node * Bst::rotateRR(Node * root)
{
    Node *rc = root->_right;
    //update root.parent.left
    if (root->_parent) {
        if (strcmp(root->_data, root->_parent->_data) > 0) {
            //root is the right child of its parent
            root->_parent->_right = root->_right;
        } else {
            root->_parent->_left = root->_right;
        }
    } else {
        //root is _root of avl, then we update _root
        _root = rc;
    }

    rc->_parent = root->_parent;
    root->_right = rc->_left;
    if (root->_right) {
        root->_right->_parent = root;
    }
    root->_parent = rc;
    rc->_left = root;

#ifdef HW_DEBUG
    cout << "after rotateRR \n";
    display();
#endif

    fixHeight(rc);
    return rc;
}

/*
 * in left child then right child
 *     (A)           (A)
 *     /             /
 *    (B)    -->    (C)  -->  (C)
 *      \           /        /   \
 *      (C)        (B)      (B)  (A)
 */

Node *Bst::rotateLR(Node * root)
{
    rotateRR(root->_left);
    return rotateLL(root);
}

/*
 * in right child then left child
 *    (A)         (A)
 *      \           \
 *     (B)    -->   (C)  -->  (C)
 *     /              \      /   \
 *   (C)              (B)   (A)  (B)
 */
Node *Bst::rotateRL(Node * root)
{
    rotateLL(root->_right);
    return rotateRR(root);
}
