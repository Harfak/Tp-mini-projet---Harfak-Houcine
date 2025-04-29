#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char name[50];
    int num;
    char pass[20];
    float money;
} Account;

Account users[MAX];
int total = 0;

int same(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

void copy(char *a, char *b) {
    int i = 0;
    while (b[i]) {
        a[i] = b[i];
        i++;
    }
    a[i] = '\0';
}

void save() {
    FILE *f = fopen("data.txt", "w");
    for (int i = 0; i < total; i++) {
        fprintf(f, "%s %d %s %.2f\n", users[i].name, users[i].num, users[i].pass, users[i].money);
    }
    fclose(f);
}

void load() {
    FILE *f = fopen("data.txt", "r");
    if (!f) return;
    while (fscanf(f, "%s %d %s %f", users[total].name, &users[total].num, users[total].pass, &users[total].money) == 4) {
        total++;
    }
    fclose(f);
}

void newAcc() {
    if (total >= MAX) return;
    char n[50], p[20];
    int num;
    printf("Name: ");
    scanf("%s", n);
    printf("Number: ");
    scanf("%d", &num);
    printf("Password: ");
    scanf("%s", p);
    copy(users[total].name, n);
    copy(users[total].pass, p);
    users[total].num = num;
    users[total].money = 0;
    total++;
    save();
    printf("Created.\n");
}

int find(int num, char *p) {
    for (int i = 0; i < total; i++) {
        if (users[i].num == num && same(users[i].pass, p)) {
            return i;
        }
    }
    return -1;
}

int findByNum(int num) {
    for (int i = 0; i < total; i++) {
        if (users[i].num == num) return i;
    }
    return -1;
}

void addMoney(int i) {
    float x;
    printf("Amount: ");
    scanf("%f", &x);
    users[i].money += x;
    save();
    printf("Done. Balance: %.2f\n", users[i].money);
}

void sendMoney(int i) {
    int n;
    float x;
    printf("To (number): ");
    scanf("%d", &n);
    int j = findByNum(n);
    if (j == -1) {
        printf("Not found.\n");
        return;
    }
    printf("Amount: ");
    scanf("%f", &x);
    if (users[i].money >= x) {
        users[i].money -= x;
        users[j].money += x;
        save();
        printf("Sent.\n");
    } else {
        printf("No money.\n");
    }
}

void seeMoney(int i) {
    printf("Balance: %.2f\n", users[i].money);
}

void removeAcc(int i) {
    for (int j = i; j < total - 1; j++) {
        users[j] = users[j + 1];
    }
    total--;
    save();
    printf("Deleted.\n");
}

void login() {
    int num;
    char p[20];
    printf("Number: ");
    scanf("%d", &num);
    printf("Password: ");
    scanf("%s", p);
    int i = find(num, p);
    if (i == -1) {
        printf("Wrong info.\n");
        return;
    }
    int c;
    do {
        printf("1. Add\n2. Send\n3. See\n4. Delete\n5. Logout\n");
        scanf("%d", &c);
        if (c == 1) addMoney(i);
        else if (c == 2) sendMoney(i);
        else if (c == 3) seeMoney(i);
        else if (c == 4) { removeAcc(i); break; }
    } while (c != 5);
}

int main() {
    load();
    int ch;
    do {
        printf("1. New\n2. Login\n3. Exit\n");
        scanf("%d", &ch);
        if (ch == 1) newAcc();
        else if (ch == 2) login();
    } while (ch != 3);
    return 0;
}