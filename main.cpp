#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

struct wordc
{
    char* word;
    int count;
};

int save_word(wordc** words, int words_count, char* word) {
    for (int i = 0; i < words_count; i++) {
        if (strcmp(word, words[i]->word) == 0) {
            words[i]->count++;
            return 0;
        }
    }
    wordc* new_word = (wordc*) malloc(sizeof(wordc));
    new_word->word = word;
    new_word->count = 1;
    words[words_count] = new_word;
    return 1;
}

char* read_word(FILE* file) {
    char letter = fgetc(file);
    while ((isspace(letter) || ispunct(letter)) && !feof(file)) {
        letter = fgetc(file);
    }
    char* word = (char*) malloc(5 * sizeof(char));
    int word_size = 5;
    int char_count = 0;
    do {
        word[char_count] = letter;
        char_count++;
        if (char_count >= word_size) {
            word_size += 5;
            word = (char*) realloc(word, word_size * sizeof(char));
        }
        letter = fgetc(file);
    } while ((isspace(letter) == 0) && (ispunct(letter) == 0) && !feof(file));
    word[char_count] = '\0';
    return word;
}

int compare_words(const void* left, const void* right) {
    wordc* left_as_wordc = *(wordc**)left;
    wordc* right_as_wordc = *(wordc**)right;
    if (left_as_wordc->count == right_as_wordc->count) {
        return strcmp(left_as_wordc->word, right_as_wordc->word);
    }
    else {
        return right_as_wordc->count - left_as_wordc->count;
    }
}

int main(int argc, char* argv[])
{
    char* filename;
    if (argc < 2) {
        //printf("please enter the file name");
        //return 0;
        filename = "kek.txt";
    }
    else {
        filename = argv[1];
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("cannot open file");
        return 0;
    }

    int words_size = 5;
    int words_count = 0;
    wordc** words = (wordc**) malloc(words_size * sizeof(wordc*));

    while (!feof(file)) {
        if (save_word(words, words_count, read_word(file))) {
            words_count++;
        }
        if (words_count >= words_size) {
            words_size += 5;
            words = (wordc**) realloc(words, words_size * sizeof(wordc*));
        }
    }

    words_count -= 1;

    qsort(words, words_count, sizeof(wordc*), compare_words);
    for (int i = 0; i < words_count; i++) {
        printf("%s: %i times\n", words[i]->word, words[i]->count);
    }
    free(words);
    fclose(file);
    return 0;
}
