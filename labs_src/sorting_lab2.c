#include <stdio.h>
#include <string.h>
#include <ctype.h>

int words_into_arr(char words[100][64]) {
    FILE *f = fopen("/Users/kostamak/CLionProjects/DSA_labs/labs_src/lab2.txt", "r");
    if (f == NULL) { return -1; }
    int c, cnt = 0, chars_cnt = 0;
    while ((c = fgetc(f)) != EOF) {
        if (!isspace(c) && !ispunct(c)) {
            words[cnt][chars_cnt++] = (char) c;
        } else {
            if (chars_cnt > 0) {
                words[cnt++][chars_cnt] = '\0';
                chars_cnt = 0;
            }
            if (cnt >= 100) { break; }
        }
    }
    if (chars_cnt > 0 && cnt < 100) {
        words[cnt++][chars_cnt] = '\0';
    }
    fclose(f);
    return cnt;
}

int str_sorting(char words[100][64], const int n) {
    for (int i = 1; i < n; i++) {
        if (words[i][0] == '\0') { continue; }
        char temp[64];
        strcpy(temp, words[i]);
        int j = i - 1;
        for (; j >= 0 && strcmp(temp, words[j]) < 0; j--) {
            strcpy(words[j + 1], words[j]);
        }
        strcpy(words[j + 1], temp);
    }
    return 0;
}

int bin_search(const char words[100][64], const int n, const char target[64]) {
    int left = 0, right = n - 1, cmp_res;
    while (left <= right) {
        const int mid = left + (right - left) / 2;
        cmp_res = strcmp(words[mid], target);
        if (cmp_res == 0) { return mid; }
        if (cmp_res < 0) { left = mid + 1; }
        else { right = mid - 1; }
    }
    return -1;
}

int main() {
    char words[100][64];
    int words_cnt = words_into_arr(words);
    if (words_cnt <= 0) { return 1; }
    str_sorting(words, words_cnt);
    char user_word[64];
    fgets(user_word, 64, stdin);
    user_word[strcspn(user_word, "\n")] = 0;
    printf("%d\n", bin_search(words, words_cnt, user_word));
    return 0;
}
