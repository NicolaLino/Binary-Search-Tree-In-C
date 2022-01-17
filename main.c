/*
 By
  ________    ___   ________   ________   ___        ________     
 |\   ___  \ |\  \ |\   ____\ |\   __  \ |\  \      |\   __  \    
 \ \  \\ \  \\ \  \\ \  \___| \ \  \|\  \\ \  \     \ \  \|\  \   
  \ \  \\ \  \\ \  \\ \  \     \ \  \\\  \\ \  \     \ \   __  \  
   \ \  \\ \  \\ \  \\ \  \____ \ \  \\\  \\ \  \____ \ \  \ \  \ 
    \ \__\\ \__\\ \__\\ \_______\\ \_______\\ \_______\\ \__\ \__\
     \|__| \|__| \|__| \|_______| \|_______| \|_______| \|__|\|__|
                                                                 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

typedef struct Dictionary // struct to save the data (number, word, meaning)
{
    int number;
    char Word[20];
    char Meaning[50];
} Dict;

struct Node// to create node of the tree
{
    Dict data;// information of the node
    struct Node *left;// pointer to left node
    struct Node *right;//pointer to right node
};


typedef struct Node *N, *BST;
BST tree = NULL; // initialize the tree to null

int isEmpty(BST root) // function to return if the tree if empty
{
    return root == NULL;
}

BST makeEmpty(BST root) // function to make the tree the empty
{
    if (root != NULL)
    {
        root->left = makeEmpty(root->left);
        root->right = makeEmpty(root->right);
        free(root);
    }
    return NULL;
}

N findMax(BST root)// function to find the maximum on the tree
{
    if (root != NULL)
    {
        if (root->right != NULL)
            return findMax(root->right);
    }
    return root;
}

N findMin(BST root)// function to find the minimum of the tree
{
    if (root != NULL)
    {
        if (root->left != NULL)
            return findMin(root->left);
    }
    return root;
}

N find(BST root, Dict data)// function to find a node in the tree and return the node
{
    if (root == NULL)
    {
        return root;
    }
    else if (strcasecmp(data.Word, root->data.Word) == 0)
        return root;
    else if (strcasecmp(data.Word, root->data.Word) > 0)
        return find(root->right, data);
    else if (strcasecmp(data.Word, root->data.Word) < 0)
        return find(root->left, data);

    //return  root;
}

N newNOde(Dict data)// function to create new node
{
    N node = (N)malloc(sizeof(struct Node));// dynamically allocate the size of the node in memory
    node->right = node->left = NULL;

    strcpy(node->data.Word, data.Word);
    strcpy(node->data.Meaning, data.Meaning);
    return node;
}

BST insert(BST root, Dict data)// function to insert new node to the binary tree
{
    if (isEmpty(root))
        return newNOde(data);
    else if (strcmp(data.Word, root->data.Word) > 0)
        root->right = insert(root->right, data);
    else if (strcmp(data.Word, root->data.Word) < 0)
        root->left = insert(root->left, data);

    return root;
}

BST delete (BST root, Dict data)// function to delete a node from the tree
{
    if (isEmpty(root))// test if the tree is empty
        printf("The node not founded in the tree");
    else if (strcasecmp(data.Word, root->data.Word) > 0)
        root->right = delete (root->right, data);// recursion
    else if (strcasecmp(data.Word, root->data.Word) < 0)
        root->left = delete (root->left, data);// recursion
    else
    {
        //if the node has No Children
        if (root->right == NULL && root->left == NULL)
        {
            free(root);
            root = NULL;
        }
        // if the node has Right Children
        else if (root->right != NULL && root->left == NULL)
        {
            N temp = root;
            root = root->right;
            free(temp);
        }
        // if the node has Left Children
        else if (root->right == NULL && root->left != NULL)
        {
            N temp = root;
            root = root->left;
            free(temp);
        }
        // if the node has two children
        else
        {
            N minRight = findMin(root->right);
            root->data = minRight->data;
            root->right = delete (root->right, minRight->data);
        }
    }

    return root;
}

void inOrder(BST root) // function to traversal the tree in order (this will sort the words on alphabetical order)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("> %s : %s\n", root->data.Word, root->data.Meaning);
        inOrder(root->right);
    }
}


void ReadFromFile()// this funciton will read from the dictionary file and then pace the file into word and meaning
{
    FILE *dictionary = fopen("dictionary.txt", "r");
    if (dictionary == NULL)// file error handling
    {
        printf("\n The file could not be opened\n\n");
        exit(1);
    }
    Dict data;
    int number, read_counter;
    char line[128];
    while (fgets(line, sizeof(line), dictionary))// while loop to loop line by line
    {
        char *string_ptr = line;
        while (sscanf(string_ptr, "%d. %[^:]: %[a-zA-Z, ]%n", &data.number, data.Word, data.Meaning, &read_counter) == 3)// sscanf to parse the line into pieces
        { //%n instead of printing or reading it actually saves the number of characters printed or read up until that point.
            int new_line = strlen(data.Meaning) - 1;// this will remove the spaces form the last character of the meaning
            if (data.Meaning[new_line] == '\n')
            {
                data.Meaning[new_line] = '\0';
            }
            tree = insert(tree, data);// insert the information on the tree
            printf("%d) > %s: %s\n", data.number, data.Word, data.Meaning);
            string_ptr += read_counter;
        }
    }
    fclose(dictionary);
}

void Heading(char heading[])// function to print the heading
{
    printf("\n___________________________________________________________________________________________________\n \t%s \n___________________________________________________________________________________________________\n", heading);
}

void search_for_word()// function to search for a word on in tree (case 1)
{
    Dict w;
    printf("\nEnter A Word :");
    scanf("%s", w.Word);
    N t = find(tree, w);
    if (t == NULL)
        printf("The Word Does Not Exist In The Dictionary.\n\n");
    else
        printf("Meaning : %s \n\n", t->data.Meaning);
}

void Update_the_meaning()// function to update the word meaning in tree (case 2)
{
    Dict w;
    printf("\nEnter A Word :");
    scanf("%s", w.Word);
    char trash;
    scanf("%c", &trash);
    N t = find(tree, w);
    if (t == NULL)
    {
        printf("The Word Does Not Exist In The Dictionary.\n\n");
    }
    else
    {
        printf("Enter A New Meaning For the Word:");
        scanf("%[^\n]", w.Meaning);
        printf("\nThe Previous Meaning : %s \n", t->data.Meaning);
        strcpy(t->data.Meaning, w.Meaning);
        printf("The New Meaning : %s \n\n", t->data.Meaning);
    }
}

void Insert_new_word()// function to insert new word in the tree (case 3)
{
    Dict w;
    printf("\nEnter A Word :");
    scanf("%s", w.Word);
    char trash;
    scanf("%c", &trash);
    printf("Enter A Meaning :");
    scanf("%[^\n]", w.Meaning);
    tree = insert(tree, w);
    printf("\n\nThe Word Was Inserted Successfully\n");
}

void Print_Words_With_Specific_Character(BST root, char c)// function to Print Words With Specific Character in tree ordered (case 5)
{
    Dict data;
    if (root != NULL)
    {
        Print_Words_With_Specific_Character(root->left, c);
        if (root->data.Word[0] == c)
        {
            printf("> %s : %s\n", root->data.Word, root->data.Meaning);
        }
        Print_Words_With_Specific_Character(root->right, c);
    }
}

void Delete_a_word()// function to delete a word in tree (case 6)
{
    Dict w;
    printf("\nEnter A Word :");
    scanf("%s", w.Word);
    N t = delete (tree, w);
    if (t == NULL)
        printf("The Word Does Not Exist In The Dictionary.\n\n");
    else
        printf("The Word Was Deleted Successfully.\n");
}

void delete_inOrder(BST root, char c)// function to delete all word that start with a specific character in tree (case 7)
{
    Dict data;
    if (root != NULL)
    {
        delete_inOrder(root->left, c);
        if (root->data.Word[0] == c)
        {
            strcpy(data.Word, root->data.Word);
            strcpy(data.Meaning, root->data.Meaning);
            delete (tree, data);
        }
        delete_inOrder(root->right, c);
    }
}

void Print_Tree_onFile(FILE *fp, BST root)// function print the tree on file ordered (case 8)
{
    if (root != NULL)
    {
        Print_Tree_onFile(fp, root->left);
        fprintf(fp, "> %s: %s\n", root->data.Word, root->data.Meaning);
        Print_Tree_onFile(fp, root->right);
    }
}

void show_menu()//function for the command window
{

    while (1)
    {

        printf("___________________________________________________________________________________________________");
        printf("\n___________________________________________________________________________________________________\n");

        printf("\nCommands:\n\n");
        // print the command
        printf("1 > Search for a word and display its meaning.\n");
        printf("2 > Update the meaning of an existing word.\n");
        printf("3 > Insert a new word from the user with its meaning. \n");
        printf("4 > Print all words in the dictionary in an alphabetic order with their associated meanings.\n");
        printf("5 > Print all words that start with a specific character in an alphabetic order.\n");
        printf("6 > Delete a word from the dictionary.\n");
        printf("7 > Delete all words that start with a specific letter.\n");
        printf("8 > Save all words back in file dictionary.txt.\n");
        printf("q > Quit\n");

        printf("\n >Enter command number to execute :");

        int command = 0;
        scanf("%d", &command);

        switch (command)//switch statement to execute the entered number
        {
            case 1:
                Heading("Search for a word");
                search_for_word();
                break;

            case 2:
                Heading("Update The Meaning");
                Update_the_meaning();
                break;

            case 3:
                Heading("Insert New Word");
                Insert_new_word();
                break;

            case 4:
                Heading("Print All Words In The Dictionary In An Alphabetic Order");
                inOrder(tree);
                break;
            case 5:
                Heading("Print all words that start with a specific character in an alphabetic order.");
                char k;
                printf("Enter A Specific Letter: ");
                scanf("%s", &k);
                printf("\n");
                Print_Words_With_Specific_Character(tree, tolower(k));
                break;
            case 6:
                Heading("Delete A Word");
                Delete_a_word();
                break;

            case 7:
                Heading("Delete all words that start with a specific letter.");
                char c;
                printf("Enter A Specific Letter: ");
                scanf("%s", &c);
                delete_inOrder(tree, tolower(c));
                printf("\nDONE!\n\n");
                break;

            case 8:
                Heading("Save all words back in file dictionary.txt.");
                FILE *out = fopen("dict.txt", "w");
                Print_Tree_onFile(out, tree);
                fclose(out);
                printf("\n\t\tDONE!\n\n");
                break;
            default:
                printf("invalid command number, quitting ...\n");
                printf("\n\n\n");
                exit(0);
        }
    }
}

int main()
{
    printf("\n\n <<HIT ENTER>> To Read The Data From The Files And Start The Process.....");
    char trash;
    scanf("%c", &trash);
    ReadFromFile();
    show_menu();
    return 0;
}
