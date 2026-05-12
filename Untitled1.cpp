// ============================================
// Name   : Amruta Pandurang Mali
// Project: Book Management System (Advanced)
// Language: C
// Features: CRUD, File Persistence, Search,
//           Sort, Top Rated, Input Validation
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
// STRUCT
// ============================================

struct Book {
    int id;
    char name[50];
    char author[50];
    char category[30];
    float price;
    float rating;
};

struct Book *b = NULL;
int count = 0;
int nextId = 101;

// ============================================
// FILE OPERATIONS
// ============================================

void saveToFile() {
    FILE *fp = fopen("books.dat", "wb");
    if (fp == NULL) {
        printf("  [ERROR] Could not open file for saving!\n");
        return;
    }
    fwrite(&count,  sizeof(int), 1, fp);
    fwrite(&nextId, sizeof(int), 1, fp);   // FIX: save nextId too
    if (count > 0)
        fwrite(b, sizeof(struct Book), count, fp);
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("books.dat", "rb");
    if (fp == NULL) return;               // no file yet — fresh start

    fread(&count,  sizeof(int), 1, fp);
    fread(&nextId, sizeof(int), 1, fp);   // FIX: restore nextId

    if (count > 0) {
        b = (struct Book *)malloc(count * sizeof(struct Book));
        if (b == NULL) {
            printf("  [ERROR] Memory allocation failed on load!\n");
            count = 0;
            fclose(fp);
            return;
        }
        fread(b, sizeof(struct Book), count, fp);
    }
    fclose(fp);
}

// ============================================
// UTILITY
// ============================================

void toLower(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

void line() {
    printf("--------------------------------------------------\n");
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================
// ADD BOOK
// ============================================

void addBook() {
    float price, rating;

    struct Book *temp = (struct Book *)realloc(b, (count + 1) * sizeof(struct Book));
    if (temp == NULL) {
        printf("  [ERROR] Memory allocation failed!\n");
        return;
    }
    b = temp;

    b[count].id = nextId++;
    printf("\n  Auto ID assigned: %d\n", b[count].id);

    printf("  Name    : ");
    scanf(" %49[^\n]", b[count].name);     // FIX: length limit

    printf("  Author  : ");
    scanf(" %49[^\n]", b[count].author);   // FIX: length limit

    printf("  Category: ");
    scanf(" %29[^\n]", b[count].category); // FIX: length limit

    do {
        printf("  Price (must be > 0): ");
        if (scanf("%f", &price) != 1) { clearInput(); price = -1; }
    } while (price <= 0);
    b[count].price = price;

    do {
        printf("  Rating (0.0 - 5.0) : ");
        if (scanf("%f", &rating) != 1) { clearInput(); rating = -1; }
    } while (rating < 0 || rating > 5);
    b[count].rating = rating;

    count++;
    saveToFile();
    printf("  [SUCCESS] Book added successfully!\n");
}

// ============================================
// DISPLAY ALL BOOKS
// ============================================

void displayBooks() {
    if (count == 0) {
        printf("  No books found. Add some books first!\n");
        return;
    }

    line();
    printf("  %-5s %-22s %-18s %-12s %-8s %-6s\n",
           "ID", "Name", "Author", "Category", "Price", "Rate");
    line();

    for (int i = 0; i < count; i++) {
        printf("  %-5d %-22s %-18s %-12s %-8.2f %-6.1f\n",
               b[i].id, b[i].name, b[i].author,
               b[i].category, b[i].price, b[i].rating);
    }
    line();
    printf("  Total Books: %d\n", count);
}

// ============================================
// SEARCH BY NAME (PARTIAL)
// ============================================

void searchByName() {
    char key[50], temp[50];
    int found = 0;

    printf("  Enter name keyword: ");
    scanf(" %49[^\n]", key);
    toLower(key);

    line();
    printf("  Search results for \"%s\":\n", key);
    line();

    for (int i = 0; i < count; i++) {
        strcpy(temp, b[i].name);
        toLower(temp);
        if (strstr(temp, key)) {
            printf("  [%d] %s by %s | Category: %s | Price: %.2f | Rating: %.1f\n",
                   b[i].id, b[i].name, b[i].author,
                   b[i].category, b[i].price, b[i].rating);
            found++;
        }
    }

    if (!found)
        printf("  No books found matching \"%s\".\n", key);
    else
        printf("  %d result(s) found.\n", found);
}

// ============================================
// SEARCH BY AUTHOR
// ============================================

void searchByAuthor() {
    char key[50], temp[50];
    int found = 0;

    printf("  Enter author name keyword: ");
    scanf(" %49[^\n]", key);
    toLower(key);

    line();
    printf("  Books by author matching \"%s\":\n", key);
    line();

    for (int i = 0; i < count; i++) {
        strcpy(temp, b[i].author);
        toLower(temp);
        if (strstr(temp, key)) {
            printf("  [%d] %s by %s | %.2f | %.1f stars\n",
                   b[i].id, b[i].name, b[i].author,
                   b[i].price, b[i].rating);
            found++;
        }
    }

    if (!found)
        printf("  No books found for author \"%s\".\n", key);
}

// ============================================
// SEARCH BY CATEGORY
// ============================================

void searchByCategory() {
    char key[30], temp[30];
    int found = 0;

    printf("  Enter category: ");
    scanf(" %29[^\n]", key);
    toLower(key);

    line();
    printf("  Books in category \"%s\":\n", key);
    line();

    for (int i = 0; i < count; i++) {
        strcpy(temp, b[i].category);
        toLower(temp);
        if (strstr(temp, key)) {
            printf("  [%d] %s by %s | %.2f | %.1f stars\n",
                   b[i].id, b[i].name, b[i].author,
                   b[i].price, b[i].rating);
            found++;
        }
    }

    if (!found)
        printf("  No books found in category \"%s\".\n", key);
}

// ============================================
// REMOVE BOOK
// ============================================

void removeBook() {
    int id, found = 0;

    printf("  Enter Book ID to delete: ");
    if (scanf("%d", &id) != 1) { clearInput(); printf("  Invalid input!\n"); return; }

    for (int i = 0; i < count; i++) {
        if (b[i].id == id) {
            printf("  Deleting: %s by %s\n", b[i].name, b[i].author);

            for (int j = i; j < count - 1; j++)
                b[j] = b[j + 1];

            count--;

            // FIX: proper null handling after realloc
            if (count == 0) {
                free(b);
                b = NULL;
            } else {
                struct Book *temp = (struct Book *)realloc(b, count * sizeof(struct Book));
                if (temp != NULL) b = temp;
            }

            saveToFile();
            printf("  [SUCCESS] Book deleted successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("  [ERROR] Book with ID %d not found!\n", id);
}

// ============================================
// UPDATE BOOK (FULL - ALL FIELDS)
// ============================================

void updateBook() {
    int id, found = 0;

    printf("  Enter Book ID to update: ");
    if (scanf("%d", &id) != 1) { clearInput(); printf("  Invalid input!\n"); return; }

    for (int i = 0; i < count; i++) {
        if (b[i].id == id) {
            printf("\n  Updating Book: %s\n", b[i].name);
            printf("  (Press Enter to keep current value)\n\n");

            char temp[50];
            clearInput();

            // FIX: update all fields, not just price/rating
            printf("  New Name [%s]: ", b[i].name);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) strncpy(b[i].name, temp, 49);

            printf("  New Author [%s]: ", b[i].author);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) strncpy(b[i].author, temp, 49);

            printf("  New Category [%s]: ", b[i].category);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) strncpy(b[i].category, temp, 29);

            float price;
            printf("  New Price [%.2f]: ", b[i].price);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                price = atof(temp);
                if (price > 0) b[i].price = price;
                else printf("  Invalid price — kept original.\n");
            }

            float rating;
            printf("  New Rating [%.1f]: ", b[i].rating);
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                rating = atof(temp);
                if (rating >= 0 && rating <= 5) b[i].rating = rating;
                else printf("  Invalid rating — kept original.\n");
            }

            saveToFile();
            printf("  [SUCCESS] Book updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("  [ERROR] Book with ID %d not found!\n", id);
}

// ============================================
// SORT BY PRICE
// ============================================

void sortByPrice(int asc) {
    if (count == 0) { printf("  No books to sort!\n"); return; }

    struct Book temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if ((asc  && b[i].price > b[j].price) ||
                (!asc && b[i].price < b[j].price)) {
                temp = b[i]; b[i] = b[j]; b[j] = temp;
            }
        }
    }

    printf("\n  Books sorted by Price (%s):\n", asc ? "Low to High" : "High to Low");
    displayBooks();
}

// ============================================
// TOP RATED BOOKS
// ============================================

void topRated() {
    if (count == 0) { printf("  No books available!\n"); return; }

    struct Book temp;
    struct Book *copy = (struct Book *)malloc(count * sizeof(struct Book));
    if (copy == NULL) { printf("  Memory error!\n"); return; }

    memcpy(copy, b, count * sizeof(struct Book));

    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (copy[i].rating < copy[j].rating) {
                temp = copy[i]; copy[i] = copy[j]; copy[j] = temp;
            }

    int show = count < 5 ? count : 5;  // show top 5
    line();
    printf("  Top %d Rated Books:\n", show);
    line();
    for (int i = 0; i < show; i++)
        printf("  %d. %-22s by %-18s | %.1f stars | Rs.%.2f\n",
               i + 1, copy[i].name, copy[i].author,
               copy[i].rating, copy[i].price);

    free(copy);
}

// ============================================
// STATISTICS
// ============================================

void showStats() {
    if (count == 0) { printf("  No books available!\n"); return; }

    float total = 0, maxP = b[0].price, minP = b[0].price;
    float maxR = b[0].rating, minR = b[0].rating;

    for (int i = 0; i < count; i++) {
        total += b[i].price;
        if (b[i].price > maxP) maxP = b[i].price;
        if (b[i].price < minP) minP = b[i].price;
        if (b[i].rating > maxR) maxR = b[i].rating;
        if (b[i].rating < minR) minR = b[i].rating;
    }

    line();
    printf("  Library Statistics\n");
    line();
    printf("  Total Books    : %d\n", count);
    printf("  Total Value    : Rs. %.2f\n", total);
    printf("  Average Price  : Rs. %.2f\n", total / count);
    printf("  Highest Price  : Rs. %.2f\n", maxP);
    printf("  Lowest Price   : Rs. %.2f\n", minP);
    printf("  Highest Rating : %.1f\n", maxR);
    printf("  Lowest Rating  : %.1f\n", minR);
    line();
}

// ============================================
// MENU
// ============================================

void menu() {
    printf("\n");
    line();
    printf("   BOOK MANAGEMENT SYSTEM — Advanced\n");
    printf("   Developed by: Amruta Pandurang Mali\n");
    line();
    printf("  [1]  Add Book\n");
    printf("  [2]  Display All Books\n");
    printf("  [3]  Search by Name\n");
    printf("  [4]  Search by Author\n");
    printf("  [5]  Search by Category\n");
    printf("  [6]  Remove Book\n");
    printf("  [7]  Update Book\n");
    printf("  [8]  Sort by Price (Low to High)\n");
    printf("  [9]  Sort by Price (High to Low)\n");
    printf("  [10] Top 5 Rated Books\n");
    printf("  [11] Library Statistics\n");
    printf("  [0]  Exit\n");
    line();
}

// ============================================
// MAIN
// ============================================

int main() {
    int choice;

    loadFromFile();
    printf("  [INFO] %d book(s) loaded from storage.\n", count);

    while (1) {
        menu();
        printf("  Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInput();
            printf("  Invalid input! Enter a number.\n");
            continue;
        }

        printf("\n");

        switch (choice) {
            case 1:  addBook();        break;
            case 2:  displayBooks();   break;
            case 3:  searchByName();   break;
            case 4:  searchByAuthor(); break;
            case 5:  searchByCategory(); break;
            case 6:  removeBook();     break;
            case 7:  updateBook();     break;
            case 8:  sortByPrice(1);   break;
            case 9:  sortByPrice(0);   break;
            case 10: topRated();       break;
            case 11: showStats();      break;
            case 0:
                if (b != NULL) free(b);
                printf("  Thank you! Goodbye.\n");
                return 0;
            default:
                printf("  Invalid choice! Please enter 0-11.\n");
        }
    }
}
