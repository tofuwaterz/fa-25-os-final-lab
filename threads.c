#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

// Define the BST

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *root = NULL;
pthread_mutex_t tree_mutex;

// Added some BST Functions

Node *create_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *insert_bst(Node *node, int value) {
    if (node == NULL)
        return create_node(value);

    if (value < node->value)
        node->left = insert_bst(node->left, value);
    else
        node->right = insert_bst(node->right, value);

    return node;
}

void inorder_traversal(Node *node) {
    if (node == NULL)
        return;

    inorder_traversal(node->left);
    printf("%d\n", node->value);
    inorder_traversal(node->right);
}

// Thread Function

void *thread_work(void *arg) {
    (void)arg;

    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)pthread_self());

    for (int i = 0; i < 100; i++) {
        int number = rand_r(&seed) % 100001;

        pthread_mutex_lock(&tree_mutex);
        root = insert_bst(root, number);
        pthread_mutex_unlock(&tree_mutex);
    }

    return NULL;
}

// Main Function which ensures that you specify the number of threads

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be positive\n");
        exit(EXIT_FAILURE);
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    if (!threads) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&tree_mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_work, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish 
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // BST is printed in order
    printf("In-order traversal of BST:\n");
    inorder_traversal(root);

    pthread_mutex_destroy(&tree_mutex);
    free(threads);

    return 0;
}
