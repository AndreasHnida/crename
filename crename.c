#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BLINK_TIMES 2
#define BLINK_DELAY 300000

void generate_random_digits(char *str, size_t length) {
  static const char digits[] = "0123456789";
  for (size_t i = 0; i < length; i++) {
    str[i] = digits[rand() % (sizeof(digits) - 1)];
  }
  // Null-terminate the string
  str[length] = '\0';
}

const char *get_file_extension(const char *filename) {
  const char *extension = strrchr(filename, '.');
  if (!extension || extension == filename)
    return "";
  return extension;
}

void animate_message() {
  const char *message = "generating random digits";
  char buffer[30];
  int message_length = strlen(message);

  for (int i = 0; i < BLINK_TIMES; ++i) {
    snprintf(buffer, sizeof(buffer), "%s", message);
    for (int j = 0; j < 4; ++j) {
      // clear the line
      printf("\r%*s", message_length + 3, "");
      printf("\r%s", buffer);
      for (int k = 0; k < j; ++k) {
        printf(".");
      }
      fflush(stdout);
      usleep(BLINK_DELAY);
    }
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", "crename");
    return 1;
  }
  const char *filename = argv[1];
  char new_filename[256];
  char random_digits[11];
  const char *extension = get_file_extension(filename);

  srand((unsigned int)time(NULL));
  animate_message();
  generate_random_digits(random_digits, 10);

  if (snprintf(new_filename, sizeof(new_filename), "%s%s", random_digits,
               extension) >= sizeof(new_filename)) {
    fprintf(stderr, "Filename too long\n");
    return 1;
  }

  if (rename(filename, new_filename) != 0) {
    perror("rename");
    return 1;
  }

  printf("File renamed to %s\n", new_filename);

  return 0;
}
