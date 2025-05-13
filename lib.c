#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_AUTHOR 100

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
} Book;

void addBook() {
    FILE *fp = fopen("library.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    printf("Enter Book ID: ");
    scanf("%d", &book.id);
    getchar();
    printf("Enter Book Title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;
    printf("Enter Book Author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    fwrite(&book, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void viewBooks() {
    FILE *fp = fopen("library.dat", "rb");
    if (!fp) {
        printf("No books found.\n");
        return;
    }

    Book book;
    printf("\n--- Book List ---\n");
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("ID: %d | Title: %s | Author: %s\n", book.id, book.title, book.author);
    }
    fclose(fp);
}

void searchBook() {
    int id, found = 0;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    FILE *fp = fopen("library.dat", "rb");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, fp)) {
        if (book.id == id) {
            printf("Book Found: ID: %d | Title: %s | Author: %s\n", book.id, book.title, book.author);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Book not found!\n");
}

void deleteBook() {
    int id, found = 0;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, fp)) {
        if (book.id != id) {
            fwrite(&book, sizeof(Book), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found) {
        printf("Book deleted successfully.\n");
    } else {
        printf("Book not found.\n");
    }
}

int main() {
    int choice;

    do {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
