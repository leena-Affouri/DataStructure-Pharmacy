#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Leena Affouri 1200335
//DR.Radi Jarrar sec.2

// to store the information of the Medicines
struct Medicines {
    char name[100];
    char activeIngredient[100];
    char category[100];
    char indication[100];
    char expiryDate[100];
    int count;
    char company[100];
    struct Medicines *left;
    struct Medicines *right;
};
#define MAX_SIZE 100

// struct to define the object variables
struct Medicine {
    char name[20];
    char ingredient[20];
    char category[20];
    char indication[50];
    char expiry_date[10];
    int count;
    char company[30];
};

// struct to define the hash table that includes an array to store elements as the hash table and the size of it
struct HashTable {
    struct Medicine data[MAX_SIZE];
    int size;
};

/*
 * function declaration
 */
struct Medicines *root = NULL;

void insert(struct Medicines *node, struct Medicines *newNode);

void buildTree();

void printInOrder(struct Medicines *node);

void printInOrderFile(struct Medicines *node, FILE *file);

struct Medicines *search(struct Medicines *node, char *name);

void listDisease(struct Medicines *node, char *disease);

struct Medicines *deleteCompany(struct Medicines *node, char *company);

struct Medicines *delete(struct Medicines *node, char *name);

void listCategory(struct Medicines *node, char *category);

void update(struct Medicines *node);

void add_new();

void hashMenu();

// menu for user
void menu() {
    FILE *file;
    char input[50];
    int option;
    // loop until user exits
    while (1) {
        puts("choose from the menu below:\n"
             "1. insert new Medicines\n"
             "2. search for a Medicines\n"
             "3. update a Medicines\n"
             "4. print the medicines in alphabetical order\n"
             "5. print all Medicines in a category\n"
             "6. print all medicines for a disease\n"
             "7. delete a Medicines\n"
             "8. delete all medicines by a single company\n"
             "9. save info to Pharmacy.txt\n"
             "10. open the hash menu\n"
             "11. exit");
        // get option from user
        scanf(" %s", input);
        option = strtol(input, NULL, 10);
        // loop until the user enetrs a correct option
        while (option < 1 || option > 11) {
            puts("enter a correct choice!");
            scanf(" %s", input);
            option = strtol(input, NULL, 10);
        }
        // make the right function call depending on user's input
        switch (option) {
            case 1:// add new Medicines
                add_new();
                puts("added successfully!");
                break;
            case 2:// searches for a Medicines
                puts("please enter the name of the Medicines: ");
                scanf(" %[^\n]", input);// read with spaces
                struct Medicines *node = search(root, input);
                if (node) {// if the Medicines exists...
                    printf("Name: %s\nActive Ingredient: %s\nCategory: %s\nIndication: %s\nExpiry Date: %s\nCount: %d\nCompany: %s\n",
                           node->name, node->activeIngredient, node->category, node->indication, node->expiryDate,
                           node->count,
                           node->company);
                } else
                    puts("the Medicines doesn't exist");
                break;
            case 3:// update the info of a certain Medicines
                puts("please enter the name of the Medicines: ");
                scanf(" %[^\n]", input);
                struct Medicines *updateNode = search(root, input);
                if (updateNode) {
                    update(updateNode);
                } else
                    puts("the Medicines doesn't exist");
                break;


            case 4:// print the tree in alphabetical order
                printInOrder(root);
                break;
            case 5:// print a category
                puts("please enter the category: ");
                scanf(" %[^\n]", input);
                listCategory(root, input);
                break;
            case 6:// print all Medicines for a specific disease
                puts("please enter the disease name: ");
                scanf(" %[^\n]", input);
                listDisease(root, input);
                break;
            case 7:// delete a Medicines with a specific name
                puts("please enter the name of the Medicines: ");
                scanf(" %[^\n]", input);
                struct Medicines *temp = search(root, input);
                if (temp) {
                    delete(root, input);
                    puts("deleted successfully!");
                } else
                    puts("the Medicines doesn't exist");
                break;
            case 8:// delete all Medicines for a specific company
                puts("please enter the name of the company: ");
                scanf(" %[^\n]", input);
                deleteCompany(root, input);
                break;
            case 9:// save info to file
                file = fopen("Pharmacy.txt", "w");
                fprintf(file, "");
                fclose(file);
                file = fopen("Pharmacy.txt", "w");
                printInOrderFile(root, file);
                fclose(file);
                printf("successful\n");
                break;
            case 10://go to phase two tp the hash
                hashMenu();
                break;
            case 11:
                exit(1);
            default:
                break;
        }
    }
}

// return the hash key (first three chars)
int hashFunction(char key[]) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += key[i];
    }
    return sum % MAX_SIZE;
}

// print the hash table including empty slots
void printTable(struct HashTable table) {
    for (int i = 0; i < MAX_SIZE; i++) {
        if (table.data[i].name[0] == '\0') {
            printf("%d: Empty\n", i);
        } else {
            printf("%d: %s\n", i, table.data[i].name);
        }
    }
}

// print the size
void printSize(struct HashTable table) {
    printf("Size of hash table: %d\n", table.size);
}


// print the used hash function
void printFunction() {
    puts("the used hash function is linear probing, where we sum the ascii code of the first three\n"
         "characters from the name as key and place the whole object inside an array, at index key % size\n"
         "of the hash table, where the size is equal to the size of the array and if there was already an\n"
         "object inside this index, we keep incrementing the index by 1, until we find an empty slot.\n"
         "hash function: (sum(first 3 chars) + i % size)");
}

// insert a new record to the hash table
void insertRecord(struct HashTable *table, struct Medicine medicine) {
    // find the hash key
    int index = hashFunction(medicine.name);
    int collisions = 0;
    // find the first empty slot
    while (table->data[index].name[0] != '\0') {
        index = (index + 1) % MAX_SIZE;
        collisions++;
    }
    // place object inside this element
    table->data[index] = medicine;
    // inc size
    table->size++;
    printf("Record inserted successfully with %d collisions.\n", collisions);
}

// searches for an element in the hash table
void searchRecord(struct HashTable table, char key[]) {
    int index = hashFunction(key);
    int collisions = 0;

    // keep looking until we match the given name
    while (strcmp(table.data[index].name, key) != 0) {
        index = (index + 1) % MAX_SIZE;
        collisions++;
        if (collisions == MAX_SIZE) {
            printf("Record not found.\n");
            return;
        }
    }
    // print record
    printf("Record found with %d collisions.\n", collisions);
    printf("Name: %s\n", table.data[index].name);
    printf("Active Ingredient: %s\n", table.data[index].ingredient);
    printf("Category: %s\n", table.data[index].category);
    printf("Indication: %s\n", table.data[index].indication);
    printf("Expiry Date: %s\n", table.data[index].expiry_date);
    printf("Count: %d\n", table.data[index].count);
    printf("Company: %s\n", table.data[index].company);
}

// delete a record from the hash table
void deleteRecord(struct HashTable *table, char key[]) {
    int index = hashFunction(key);
    int collisions = 0;
    // keep looking until the name is found
    while (strcmp(table->data[index].name, key) != 0) {
        index = (index + 1) % MAX_SIZE;
        collisions++;
        if (collisions == MAX_SIZE) {
            printf("Record not found.\n");
            return;
        }
    }
    // null the element if found (record)
    table->data[index].name[0] = '\0';
    table->size--;
    printf("rehashing...");
    struct HashTable new_table;
    new_table.size = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        new_table.data[i].name[0] = '\0';
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        if (table->data[i].name[0] != '\0')
            insertRecord(&new_table, table->data[i]);
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        table->data[i] = new_table.data[i];
    }
    printf("Record deleted successfully.\n");
}

// save the hash table into a file
void saveData(struct HashTable table) {
    FILE *file = fopen("Pharmacy.txt", "w");
    for (int i = 0; i < MAX_SIZE; i++) {
        if (table.data[i].name[0] != '\0') {
            fprintf(file, "%s:%s:%s:%s:%s:%d:%s\n", table.data[i].name, table.data[i].ingredient,
                    table.data[i].category,
                    table.data[i].indication, table.data[i].expiry_date, table.data[i].count, table.data[i].company);
        }
    }
    fclose(file);
    printf("Data saved to file successfully.\n");
}

void hashMenu() {
    // define table
    struct HashTable table;
    table.size = 0;
    // initialize the table
    for (int i = 0; i < MAX_SIZE; i++) {
        table.data[i].name[0] = '\0';
    }
    // Loading data from file
    FILE *file = fopen("Pharmacy.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char line[200];

    // read file line by line
    while (fgets(line, sizeof(line), file)) {
        struct Medicine medicine;
        // split the lines
        sscanf(line, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\n]", medicine.name, medicine.ingredient, medicine.category,
               medicine.indication, medicine.expiry_date, &medicine.count, medicine.company);
        insertRecord(&table, medicine);
    }
    fclose(file);

    // loop until the user exits
    int choice = 0;
    while (choice != 8) {
        char key[20];
        struct Medicine medicine;
        char count[5];

        printf("\nMenu:\n");
        printf("1. Print hashed table\n");
        printf("2. Print the size of hash table\n");
        printf("3. Print the used hash function\n");
        printf("4. Insert a new record\n");
        printf("5. Search for a specific medicine\n");
        printf("6. Delete a specific record\n");
        printf("7. Save the data back to the file\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printTable(table);
                break;
            case 2:
                printSize(table);
                break;
            case 3:
                printFunction();
                break;
            case 4:
                printf("Enter name: ");
                scanf(" %[^\n]", medicine.name);
                printf("Enter active ingredient: ");
                scanf(" %[^\n]", medicine.ingredient);
                printf("Enter category: ");
                scanf(" %[^\n]", medicine.category);
                printf("Enter indication: ");
                scanf(" %[^\n]", medicine.indication);
                printf("Enter expiry date: ");
                scanf(" %[^\n]", medicine.expiry_date);
                printf("Enter count: ");
                scanf(" %[^\n]", count);
                medicine.count = strtol(count, NULL, 10);
                printf("Enter company: ");
                scanf(" %[^\n]", medicine.company);
                insertRecord(&table, medicine);
                break;
            case 5:
                printf("Enter the name of medicine to search: ");
                scanf(" %[^\n]", key);
                searchRecord(table, key);
                break;
            case 6:
                printf("Enter the name of medicine to delete: ");
                scanf(" %[^\n]", key);
                deleteRecord(&table, key);
                break;
            case 7:
                saveData(table);
                break;
            case 8:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid choice.\n");
                break;
        }
    }
}

// main to build tree and execute the menu
int main() {
    buildTree();
    menu();
    hashMenu();

    return 0;
}

// function to update a node from user
void update(struct Medicines *node) {
    char newName[100], newActiveIngredient[100], newCategory[100], newIndication[100],
            newExpiryDate[100], newCompany[100], newCount[10];
    printf("Enter the new name: ");
    scanf(" %[^\n]", newName);
    printf("Enter the new active ingredient: ");
    scanf(" %[^\n]", newActiveIngredient);
    printf("Enter the new category: ");
    scanf(" %[^\n]", newCategory);
    printf("Enter the new indication: ");
    scanf(" %[^\n]", newIndication);
    printf("Enter the new expiry date: ");
    scanf(" %[^\n]", newExpiryDate);
    printf("Enter the new count: ");
    scanf(" %s", newCount);
    printf("Enter the new company: ");
    scanf(" %[^\n]", newCompany);
    strcpy(node->name, newName);
    strcpy(node->activeIngredient, newActiveIngredient);
    strcpy(node->category, newCategory);
    strcpy(node->indication, newIndication);
    strcpy(node->expiryDate, newExpiryDate);
    node->count = strtol(newCount, NULL, 10);
    strcpy(node->company, newCompany);
}

// returns the minimum of the right node
struct Medicines *minValueNode(struct Medicines *node) {
    struct Medicines *current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// delete all nodes to the same company
struct Medicines *deleteCompany(struct Medicines *node, char *company) {
    // if head == NULL
    if (node == NULL) {
        return NULL;
    }
    // iterate through the whole tree recursively and delete every node
    node->left = deleteCompany(node->left, company);
    node->right = deleteCompany(node->right, company);
    // if a node is found
    if (strcmp(node->company, company) == 0) {
        if (node->left == NULL) {// If the node has no left child, the right child replaces it.
            struct Medicines *temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) { // otherwise
            struct Medicines *temp = node->left;
            free(node);
            return temp;
        }
        // if we have two children the smallest value in the right subtree is
        // copied to the node and the smallest value in the right subtree is deleted.
        struct Medicines *temp = minValueNode(node->right);
        strcpy(node->name, temp->name);
        strcpy(node->activeIngredient, temp->activeIngredient);
        strcpy(node->category, temp->category);
        strcpy(node->indication, temp->indication);
        strcpy(node->expiryDate, temp->expiryDate);
        node->count = temp->count;
        strcpy(node->company, temp->company);
        node->right = delete(node->right, temp->name);
    }
    return node;
}

// same as the above function
struct Medicines *delete(struct Medicines *node, char *name) {
    if (node == NULL) {
        puts("tree is empty");
        return NULL;
    }
    if (strcmp(name, node->name) < 0) {
        node->left = delete(node->left, name);
    } else if (strcmp(name, node->name) > 0) {
        node->right = delete(node->right, name);
    } else {
        if (node->left == NULL) {
            struct Medicines *temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            struct Medicines *temp = node->left;
            free(node);
            return temp;
        }
        struct Medicines *temp = minValueNode(node->right);
        strcpy(node->name, temp->name);
        strcpy(node->activeIngredient, temp->activeIngredient);
        strcpy(node->category, temp->category);
        strcpy(node->indication, temp->indication);
        strcpy(node->expiryDate, temp->expiryDate);
        node->count = temp->count;
        strcpy(node->company, temp->company);
        node->right = delete(node->right, temp->name);
    }
    return node;
}

// iterate the tree recursively and print any node with the same category
void listCategory(struct Medicines *node, char *category) {
    if (node != NULL) {
        listCategory(node->left, category);
        if (strcmp(node->category, category) == 0) {
            printf("Name: %s\nActive Ingredient: %s\nCategory: %s\nIndication: %s\nExpiry Date: %s\nCount: %d\nCompany: %s\n",
                   node->name, node->activeIngredient, node->category, node->indication, node->expiryDate, node->count,
                   node->company);
            if (node->count > 0)
                puts("Available: yes");
            else
                puts("Available: No");
            puts("_____________________________________________________");
        }
        listCategory(node->right, category);
    }
}

// iterate the tree and return the node if found otherwise return NULL
struct Medicines *search(struct Medicines *node, char *name) {
    if (node == NULL) {
        return NULL;
    } else if (strcmp(name, node->name) == 0) {
        return node;
    } else if (strcmp(name, node->name) < 0) {
        return search(node->left, name);
    } else {
        return search(node->right, name);
    }
}

// iterate the tree recursively and print any node with the same disease
void listDisease(struct Medicines *node, char *disease) {
    if (node != NULL) {
        listDisease(node->left, disease);
        if (strstr(node->indication, disease) != NULL) {
            printf("Name: %s\nActive Ingredient: %s\nCategory: %s\nIndication: %s\nExpiry Date: %s\nCount: %d\nCompany: %s\n",
                   node->name, node->activeIngredient, node->category, node->indication, node->expiryDate, node->count,
                   node->company);
            puts("_____________________________________________________");
        }
        listDisease(node->right, disease);
    }
}

// read a new node from user and inserts it using the insert function
void add_new() {
    struct Medicines *newNode = (struct Medicines *) malloc(sizeof(struct Medicines));
    char input[10];
    puts("name: ");
    scanf(" %s", newNode->name);
    puts("Active Ingredient: ");
    scanf(" %s", newNode->activeIngredient);
    puts("Category: ");
    scanf(" %s", newNode->category);
    puts("Indication: ");
    scanf(" %s", newNode->indication);
    puts("Expiry Date: ");
    scanf(" %s", newNode->expiryDate);
    puts("Count: ");
    scanf(" %s", input);
    newNode->count = strtol(input, NULL, 10);
    puts("Company: ");
    scanf(" %s", newNode->company);
    insert(root, newNode);

}

// iterate through the tree and print it
void printInOrder(struct Medicines *node) {
    if (node != NULL) {
        printInOrder(node->left);
        printf("Name: %s\nActive Ingredient: %s\nCategory: %s\nIndication: %s\nExpiry Date: %s\nCount: %d\nCompany: %s\n",
               node->name, node->activeIngredient, node->category, node->indication, node->expiryDate, node->count,
               node->company);
        puts("_____________________________________________________");
        printInOrder(node->right);
    }
}

// iterate through the tree and print it using "a", which appends to text to the file instead of rewriting it
void printInOrderFile(struct Medicines *node, FILE *file) {

    if (node != NULL) {
        printInOrderFile(node->left, file);
        fprintf(file,
                "%s:%s:%s:%s:%s:%d:%s\n",
                node->name, node->activeIngredient, node->category, node->indication, node->expiryDate, node->count,
                node->company);
        printInOrderFile(node->right, file);
    }

}

// insert a new node in its right place inside the tree
void insert(struct Medicines *node, struct Medicines *newNode) {

    if (root == NULL) {
        root = newNode;
    } else if (strcmp(newNode->name, node->name) <
               0) { // if less go left and if left is null insert new node else go left
        if (node->left == NULL) {
            node->left = newNode;
        } else {
            insert(node->left, newNode);
        }
    } else {
        if (node->right == NULL) {// otherwise
            node->right = newNode;
        } else {
            insert(node->right, newNode);
        }
    }
}

void buildTree() {// loop through the file and store the data inside a tree using insert function
    FILE *file = fopen("Meds.txt", "r");
    //Print a message if the file is not found
    if (file == NULL)
    {
        printf("Sorry :(\nFile doesn't exist");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct Medicines *newNode = (struct Medicines *) malloc(sizeof(struct Medicines));
        sscanf(line, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\n]", newNode->name, newNode->activeIngredient,
               newNode->category, newNode->indication, newNode->expiryDate, &newNode->count, newNode->company);
        newNode->left = NULL;
        newNode->right = NULL;
        insert(root, newNode);
    }
    fclose(file);
}