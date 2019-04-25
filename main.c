#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Linked List.c"

/*
 * Criteria:
 *  Array of linked lists > Hash Table (HT)
 *  Node (struct) - char[] name, int age
 *  Hash value computation
 *  Create a file of names
 *  Read from file and create starting hash table
 */

/*6
 * Paper and pencil analysis: Hash value
 *  HV = SUM(
 *      for i in len(name):
 *          mod(asciitoint(name[i]), table_size)
 *  )
 *
 *  char name[25];
 *  scanf("%s", name);
 *
 *  T: name[0] - 84
 *  O: name[1] - 111
 *  M: name[2] - 109
 *
 *  for (int i = 0; i < len(name); i++) {
 *      sum += name[i];
 *  }
 *
 *  HV = sum % table_size;
 *
 *  i | sum | array[i] (data) | HV
 *  0 | 84  | 84              | /
 *  1 | 195 | 111             | /
 *  2 | 304 | 109             | /
 *  3 | /   | /               | 304 % 5 = 4
 *
 *  Hash Table
 *  |---------------------------------------|
 *  | 0: H1 | 1: H2 | 2: H3 | 3: H4 | 4: H5 |
 *  |---------------------------------------|
 *
 *  H1 = 0 | NULL
 *  H2 = 0 | NULL
 *  H3 = 0 | NULL
 *  H4 = 1 | ->"Tom" | 54 | NULL
 *  H5 = 0 | NULL
 *
 *  Jane
 *  Mike
 *  Don
 *  Al
 *  Sue
 *
 */

int sum_string(char string[], int string_length);
node_t * add_node_table(header_t hash_table[], data_t *data, int table_size);
int delete_node_table(header_t hash_table[], data_t *data);
int print_table_list(header_t *head[], int table_size);
node_t * search_table(header_t head[], data_t *data);

int main(void) {

    // Get table size from the user and check if it's valid
    int table_size;
    do {
        // Get user input
        printf("What is the size of the hash table?\n> ");
        scanf(" %d", &table_size);

        // Check if table_size is a valid array size
        if (table_size < 1) {
            printf("Please enter a number greater than 0.\n");
            continue;
        }
    } while (table_size < 1);

    // Create the hash table with the user input table size
    header_t *hash_table[table_size];

    // Fill table with default values
    for (int counter = 0; counter < table_size; counter++) {
        hash_table[counter] = (header_t *) malloc(sizeof(header_t));

        hash_table[counter]->first_node_ptr = NULL;
        hash_table[counter]->node_counter = 0;
    }

    printf("Welcome to the hash table!\n");

    int option;
    do {

        printf("------------------\n");
        printf("What would you like to do?\n"
               "0: Exit\n"
               "1: Add node\n"
               "2: Delete node\n"
               "3: Print list\n"
               "4: Search the list\n"
               "> "
        );
        scanf(" %d", &option);
        printf("------------------\n");

        data_t *new_data = (data_t *) malloc(sizeof(data_t));
        node_t *searched;
        int deleted;
        int order;
        switch (option) {
            // Add a node
            case 1:
                new_data = (data_t *) malloc(sizeof(data_t));
                printf("What is the name of the person?\n> ");
                scanf(" %s", new_data->name);
                printf("What is the persons age?\n> ");
                scanf(" %d", &new_data->age);

                order = sum_string(new_data->name, strlen(new_data->name)) % table_size;

                add_node_table(hash_table[order], new_data, table_size);
                break;
            case 2:
                printf("Who do you want to delete? (name)\n> ");
                scanf(" %s", new_data->name);
                printf("How old are they? (#)\n> ");
                scanf(" %d", &new_data->age);

                order = sum_string(new_data->name, strlen(new_data->name)) % table_size;

                deleted = delete_node_table(hash_table[order], new_data);

                if (deleted == -1 || deleted == -2) {
                    printf("Node not found\n");
                    break;
                }

                printf("Node delete at: #%d\n", deleted);

                break;
            case 3:
                print_table_list(hash_table, table_size);
                break;
            case 4:
                printf("Who do you want to find? (name)\n> ");
                scanf(" %s", new_data->name);
                printf("What is their age? (#)\n> ");
                scanf(" %d", &new_data->age);

                order = sum_string(new_data->name, strlen(new_data->name)) % table_size;

                searched = search_table(hash_table[order], new_data);

                if (searched == NULL) {
                    printf("Node not found\n");
                    break;
                }

                printf("Found %s, age %d!\n", ((searched->data)->name), (searched->data)->age);
                break;

        }

    } while (option != 0);

    // End program - return success
    return 0;
}

int sum_string(char string[], int string_length) {


    // Add each number to sum
    int sum = 0;
    for (int counter = 0; counter < string_length; counter++) {
        sum += string[counter];
    }

    // Stop the function - return sum of characters
    return sum;
}

node_t * add_node_table(header_t hash_table[], data_t *data, int table_size) {

    // Find the order
    int new_sum = sum_string(data->name, strlen(data->name));
    int order = new_sum % table_size;

    // Return the new node
    return add_node(hash_table, data, order);
}

int delete_node_table(header_t *table, data_t *data) {

    // Call delete_node with the correct table and the order integer
    int deleted = delete_node(table, data);

    // If deleted is not -1 or -2, at least 0
    if (deleted != -1 && deleted != -2) {
        return deleted;
    }

    // If the node wasn't found - return failure
    return -1;

}

int print_table_list(header_t *head[], int table_size) {

    for (int counter1 = 0; counter1 < table_size; counter1++) {
        node_t *current_node_ptr = head[counter1]->first_node_ptr;
        printf("Table #%d:\n", counter1);
        for (int counter2 = 0; current_node_ptr != NULL; counter2++) {

            printf("\t %s, %d\n", (current_node_ptr->data)->name, (current_node_ptr->data)->age);

            current_node_ptr = current_node_ptr->next_node_ptr;
        }
    }

    return 0;
}

node_t * search_table(header_t head[], data_t *data) {

    // Call search_list with each header and the order number
    node_t * found_ptr = search_list(*head, data);

    // If found isn't NULL
    if (found_ptr != NULL) {
        // Return found node
        return found_ptr;
    }

    return NULL;
}
