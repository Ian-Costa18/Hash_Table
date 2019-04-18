#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Create node and header types
typedef struct node_t {
    int data;
    int position;
    struct node_t *next_node_ptr;
} node_t;

typedef struct header_t {
    node_t *first_node_ptr;
    int node_counter;
} header_t;

// Function headers
void start_menu(header_t *head);
node_t * add_node(header_t *head, int data);
int delete_node(header_t *head, int data);
int print_list(header_t head);
node_t * search_list(header_t head, int data);

/*
int main(void) {

    // Create the header for the linked list
    header_t header;
    // Set starting values for header
    header.first_node_ptr = NULL;
    header.node_counter = 0;

    start_menu(&header);

    // End the program - return success
    return 0;
}
*/

void start_menu(header_t *head) {

    // Start the menu
    int option;
    printf("Welcome to the linked list!\n");
    // Start menu loop, run once before checking conditional
    do {
        printf("--------------------------\n");
        printf("What would you like to do?\n");
        printf("0: Exit\n"
               "1: Add a node\n"
               "2: Delete a node\n"
               "3: Print the list\n"
               "4: Search the list for a node with given data\n"
               "> ");
        scanf(" %d", &option);
        printf("--------------------------\n");

        // Choose which function to use based on options
        if (option == 1) {

            // Ask for new nodes data
            int add_data;
            printf("What data is for this node?\n> ");
            scanf(" %d", &add_data);

            // Call the add node function
            node_t *new_node = add_node(head, add_data);

            printf("Node created at position #%d\n", new_node->position);
        } else if (option == 2) {

            // Loop until data is found, run at least once
            char cont;
            do {
                // Find what node to delete
                int delete_data;
                printf("What piece of data should be deleted?\n> ");
                scanf(" %d", &delete_data);
                // Call the delete node function
                int deleted_node = delete_node(head, delete_data);

                // If node isn't found, ask to continue
                if (deleted_node == -1) {
                    printf("Node not found, try again? (y/n)\n> ");
                    scanf(" %c", &cont);
                    continue;
                } else if (deleted_node == -2) {
                    // If list is empty, alert user
                    printf("List is empty, nothing deleted\n");
                    continue;
                }

                // Tell the user what node was deleted
                printf("Deleted node #%d\n", deleted_node);

            } while (cont == 'y');

        } else if (option == 3) {

            // Call the print list function
            print_list(*head);
        } else if (option == 4) {

            // Ask the user to input data to search for
            int search_data;
            printf("What data do you want to find?\n> ");
            scanf(" %d", &search_data);

            // Call the search_list function, assign the output to found_node
            node_t *found_node = search_list(*head, search_data);

            // Check if node was not found
            if (found_node == NULL) {

                printf("No node with that data found in the list\n");
                continue;
            }

            // If a node was found, print it
            printf("Node found at position #%d!\n", found_node->position);
        } else {
            // If input wasn't recognized, continue
            // Stops the loop if input was 0
            continue;
        }

    } while (option != 0);
}

node_t * add_node(header_t *head, int data) {
    // Add's a node to the linked list in ascending order

    // Dereference head's first pointer to get first node
    node_t *current_node_ptr = head->first_node_ptr;

    // Create new node as new_node_ptr
    node_t *new_node_ptr = (node_t *) malloc(sizeof(node_t));
    node_t *previous_node_ptr = current_node_ptr;

    // Add data to the new node, set next node to nothing
    new_node_ptr->data = data;
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
    if (new_node_ptr->data < current_node_ptr->data) {
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
        if (new_node_ptr->data < current_node_ptr->data) {

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

int delete_node(header_t *head, int data) {
    // Delete a node based on its data
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
    if (data == current_node_ptr->data) {

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
        if (data == current_node_ptr->data) {

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

        } else if (data < current_node_ptr->data) {
            // List is ordered, if data is less than the current node's data
            // the data must not have been found
            break;
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

int print_list(header_t head) {
    // Prints the linked list starting from the head
    // Returns 0 if successful

    // Get the first node from head
    node_t *node_ptr = head.first_node_ptr;

    // Print the list, start node_counter at 1 and increase after every loop
    // Only run while node is not NULL
    for (int counter = 0; node_ptr != NULL; counter++) {

        printf("Node #%d: %d\n", node_ptr->position, node_ptr->data);

        node_ptr = node_ptr->next_node_ptr;
    }

    // When the loop ends, say it
    printf("No more nodes\n");

    // Stop the function - return success
    return 0;
}

node_t * search_list(header_t head, int data) {
    // Searches the list for a node based on its data
    // Returns pointer to the node, or NULL if node wasn't found

    // Get the first node from head
    node_t *node_ptr = head.first_node_ptr;

    // Crawl through the list with counter
    for (int counter = 0; node_ptr != NULL; counter++) {

        // Check if node's data equals the data we're searching for
        if (node_ptr->data == data) {

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
