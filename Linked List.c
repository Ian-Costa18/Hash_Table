#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create node and header types

typedef struct data_t {
    char name[32];
    int age;
} data_t;

typedef struct node_t {
    data_t *data;
    int order;
    int position;
    struct node_t *next_node_ptr;
} node_t;

typedef struct header_t {
    node_t *first_node_ptr;
    int node_counter;
} header_t;

// Define functions
node_t * add_node(header_t *head, data_t *data, int order) {
    // Add's a node to the linked list in ascending order
    //

    // Dereference head's first pointer to get first node
    node_t *current_node_ptr = head->first_node_ptr;

    // Create new node as new_node_ptr
    node_t *new_node_ptr = (node_t *) malloc(sizeof(node_t));
    node_t *previous_node_ptr = current_node_ptr;

    // Add data to the new node, set next node to nothing
    new_node_ptr->data = data;
    new_node_ptr->order = order;
    new_node_ptr->next_node_ptr = NULL;

    // Check if there's no first node
    if (current_node_ptr == NULL) {

        // Set node's position to 0
        new_node_ptr->position = 0;
        // Add first node to head's pointer
        head->first_node_ptr = new_node_ptr;
        head->node_counter++; // Increase node headcount

        // Stop the function - return the new node
        return new_node_ptr;
    }

    // If we're replacing the first node
    if (new_node_ptr->order < current_node_ptr->order) {
        // Set node's position to 0
        new_node_ptr->position = 0;
        // Set new node's pointer to current node
        new_node_ptr->next_node_ptr = current_node_ptr;
        // Replace the first node
        head->first_node_ptr = new_node_ptr;
        head->node_counter++; // Increase node headcount

        // Fix the positions for the rest of the nodes
        for (int fixed_position = 1; current_node_ptr != NULL; fixed_position++) {
            current_node_ptr->position = fixed_position;
            current_node_ptr = current_node_ptr->next_node_ptr;
        }

        // stop the function - return the new node
        return new_node_ptr;
    }

    // Crawl through the list to find the new nodes place
    int counter; // Start a counter for node.position
    for (counter = 0; current_node_ptr != NULL; counter++) {

        // If the new node's data is greater than the current node's data, replace it
        if (new_node_ptr->order < current_node_ptr->order) {

            // Set new node's position to current position
            new_node_ptr->position = counter;

            // Set new node's next pointer to current node
            new_node_ptr->next_node_ptr = current_node_ptr;
            // Set previous node's position to counter - 1
            previous_node_ptr->position = counter-1;
            // Set previous node's pointer to the new node
            previous_node_ptr->next_node_ptr = new_node_ptr;
            head->node_counter++; // Increase node headcount

            // Fix the positions for the rest of the nodes
            for (int fixed_position = counter+1; current_node_ptr != NULL; fixed_position++) {
                current_node_ptr->position = fixed_position;
                current_node_ptr = current_node_ptr->next_node_ptr;
            }

            // Stop the function after replacing the node - Return the new node
            return new_node_ptr;
        }

        // Set the last node to current node
        previous_node_ptr = current_node_ptr;
        // Keep setting node to next node until the next node is NULL
        current_node_ptr = current_node_ptr->next_node_ptr;
    }

    // Set new node's position to the last position
    new_node_ptr->position = counter;
    // Place the node at the end
    previous_node_ptr->next_node_ptr = new_node_ptr;
    head->node_counter++; // Increase node headcount

    // Stop the function - return the new node
    return new_node_ptr;
}

int delete_node(header_t *head, data_t *data) {
    // Delete a node based on its order data
    // Returns -2 if list is empty, -1 if data isn't found, or,
    // if a node was deleted, the place in the list the node was deleted from

    // Dereference the head node
    node_t *current_node_ptr = head->first_node_ptr;
    // Declare prev_node
    node_t *previous_node_ptr = current_node_ptr;

    // Check if list is empty first
    if (current_node_ptr == NULL) {
        // Stop the function - return list is empty
        return -2;
    }

    // Check if the first node is going to be deleted
    if (strcmp(data->name, (current_node_ptr->data)->name) >= 0 && data->age == (current_node_ptr->data)->age) {

        // Assign the previous node to first node
        previous_node_ptr = head->first_node_ptr;
        // Set the first node to the second node
        head->first_node_ptr = current_node_ptr->next_node_ptr;
        // Free the first node
        free(previous_node_ptr);

        head->node_counter--; // Decrement head node_counter

        // Fix the positions for the rest of the nodes
        for (int fixed_position = 1; current_node_ptr != NULL; fixed_position++) {
            current_node_ptr->position = fixed_position;
            current_node_ptr = current_node_ptr->next_node_ptr;
        }

        // Stop the function - return 0 for the first node
        return 0;
    }

    // Loop through the list until node is NULL
    // Create a counter called node_position to track which node gets deleted
    for (int node_position = 0; current_node_ptr != NULL; node_position++) {

        // Check if current node should be deleted
        if (strcmp(data->name, (current_node_ptr->data)->name) >= 0 && data->age == (current_node_ptr->data)->age) {

            // Change the last node to point to the next node
            previous_node_ptr->next_node_ptr = current_node_ptr->next_node_ptr;
            // Free the current node
            free(current_node_ptr);

            head->node_counter--; // Decrement head node_counter

            // Fix the positions for the rest of the nodes
            for (int fixed_position = node_position-1; current_node_ptr != NULL; fixed_position++) {
                current_node_ptr->position = fixed_position;
                current_node_ptr = current_node_ptr->next_node_ptr;
            }

            // Stop the function - return position of node in list
            return node_position;

        }

        // Check if this is last node
        if (current_node_ptr->next_node_ptr == NULL) {
            // Stop the loop
            break;
        }

        // Set the last node to current node
        previous_node_ptr = current_node_ptr;
        // Set current node to next node
        current_node_ptr = current_node_ptr->next_node_ptr;
    }

    // Stop the function - return node not found
    return -1;
}

node_t * search_list(header_t head, data_t *data) {
    // Searches the list for a node based on its order
    // Returns pointer to the node, or NULL if node wasn't found

    // Get the first node from head
    node_t *node_ptr = head.first_node_ptr;

    // Crawl through the list with counter
    for (int counter = 0; node_ptr != NULL; counter++) {

        // Check if node's data equals the data we're searching for
        if (data->age == (node_ptr->data)->age) {

            // Stop the function - return the found node
            return node_ptr;
        }

        // Set node to next node
        node_ptr = node_ptr->next_node_ptr;
    }

    // If nothing was found
    // Stop the function - return node not found
    return NULL;
}
