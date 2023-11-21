#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bst_node *bst;

struct bst_node {
    int value;
    bst left, right;
};

void bst_print_elements(bst tree) {
    if (tree != NULL) {
        bst_print_elements(tree->left);
        printf("%d, ", tree->value);
        bst_print_elements(tree->right);
    }
}

void bst_print(bst tree) {
    printf("{");
    bst_print_elements(tree);
    printf("}\n");
}

bool bst_insert(bst *tree, int value) {
    // TODO: Task 1: insert 'value' into 'tree'.
    // If the tree is empty, create a new node and make it the root
    if (*tree == NULL) {
        *tree = (bst)malloc(sizeof(struct bst_node));
        if (*tree == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        (*tree)->value = value;
        (*tree)->left = (*tree)->right = NULL;
        return true;  // Node inserted successfully
    }

    bst current = *tree;
    bst parent = NULL;

    // Search for the correct position to insert the new value
    while (current != NULL) {
        if (value < current->value) {
            parent = current;
            current = current->left;
        } else if (value > current->value) {
            parent = current;
            current = current->right;
        } else {
            return false;  // Value already exists in the tree
        }
    }

    // Create a new node and link it to the tree
    bst new_node = (bst)malloc(sizeof(struct bst_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->left = new_node->right = NULL;

    // Attach the new node to the parent
    if (value < parent->value) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    return true;  // Node inserted successfully
}

// Helper function to find the node with the minimum value in a BST
bst find_min(bst node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}
bool bst_remove(bst *tree, int value) {
    // TODO: Task 2: remove 'value' from 'tree'.
    // If the tree is empty, the value is not present
    if (*tree == NULL) {
        return false;
    }

    bst current = *tree;
    bst parent = NULL;

    // Search for the node to be removed
    while (current != NULL && current->value != value) {
        parent = current;
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // If the value is not present in the tree
    if (current == NULL) {
        return false;
    }

    // Case 1: Node with only one child or no child
    if (current->left == NULL) {
        bst temp = current->right;
        free(current);
        if (parent == NULL) {
            *tree = temp;
        } else if (current == parent->left) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
    } else if (current->right == NULL) {
        bst temp = current->left;
        free(current);
        if (parent == NULL) {
            *tree = temp;
        } else if (current == parent->left) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
    } else {
        // Case 3: Node with two children
        bst successor = find_min(current->right);
        current->value = successor->value;
        bst_remove(&(current->right), successor->value);
    }

    return true; // Node removed successfully
}

void bst_union_helper(bst tree, bst *union_tree) {
    if (tree != NULL) {
        bst_union_helper(tree->left, union_tree);

        // Use the bst_insert function to insert elements into the union tree
        bst_insert(union_tree, tree->value);

        bst_union_helper(tree->right, union_tree);
    }
}
bst bst_union(bst tree1, bst tree2) {
    // TODO: Task 3: compute the union of 'tree1' and 'tree2'.
    bst union_tree = NULL;

    // Traverse the first tree and insert elements into the union tree
    bst_union_helper(tree1, &union_tree);

    // Traverse the second tree and insert elements into the union tree
    bst_union_helper(tree2, &union_tree);

    return union_tree;
}

// Find if there is tree_val in tree.
void bst_find_elements(bst tree, int tree_val,bool *flag) {
    if (tree != NULL) {
        bst_find_elements(tree->left,tree_val,flag);
        if (tree->value == tree_val)
            *flag = true;
        bst_find_elements(tree->right,tree_val,flag);
    }
}

// Helper function for intersection - traverses both input trees and inserts common elements into the intersection tree
void bst_intersection_helper(bst tree1, bst tree2, bst *intersection_tree) {
    if ( tree2 != NULL) {
        bool flag = false;
        bst_find_elements(tree1,tree2->value,&flag);
        if (flag) {
            bst_insert(intersection_tree, tree2->value);
            bst_intersection_helper(tree1,tree2->left,intersection_tree);
            bst_intersection_helper(tree1,tree2->right,intersection_tree);
        } else{
            bst_intersection_helper(tree1,tree2->left,intersection_tree);
            bst_intersection_helper(tree1,tree2->right,intersection_tree);
        }
    }
}

bst bst_intersection(bst tree1, bst tree2) {
    // TODO: Task 3: compute the intersection of 'tree1' and 'tree2'.
    bst intersection_tree = NULL;
    // Traverse both trees and insert common elements into the intersection tree
    bst_intersection_helper(tree1, tree2, &intersection_tree);
    return intersection_tree;
}

int main() {
    // Initialize two trees.
    bst tree1 = NULL, tree2 = NULL;

    assert(bst_insert(&tree1, 1));
    assert(bst_insert(&tree1, 9));
    assert(bst_insert(&tree1, 2));
    assert(bst_insert(&tree1, 7));
    assert(bst_insert(&tree1, 0));
    assert(bst_insert(&tree1, 3));
    assert(bst_insert(&tree1, 13));
    assert(bst_insert(&tree1, 5));
    assert(!bst_insert(&tree1, 3));

    // Should print: {0, 1, 2, 3, 5, 7, 9, 13, }
    bst_print(tree1);

    assert(bst_insert(&tree2, 12));
    assert(bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 1));
    assert(bst_insert(&tree2, 0));
    assert(bst_insert(&tree2, 19));
    assert(bst_insert(&tree2, 7));
    assert(!bst_insert(&tree2, 13));
    assert(bst_insert(&tree2, 5));

    // Should print: {0, 1, 5, 7, 12, 13, 19, }
    bst_print(tree2);

    // Should print: {0, 1, 2, 3, 5, 7, 9, 12, 13, 19, }
    bst union_1_2 = bst_union(tree1, tree2);
    bst_print(union_1_2);

    // Should print: {0, 1, 5, 7, 13, }
    bst intersection_1_2 = bst_intersection(tree1, tree2);
    bst_print(intersection_1_2);

    return 0;
}
