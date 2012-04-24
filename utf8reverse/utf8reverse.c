#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int get_mbseq_len (char *utf8buf)
{
    unsigned char byte = utf8buf[0];
    int copylen = -1;

    if ((byte&0xF0)==0xF0) { /* 4-byte char copy */
        copylen = 4;
    } else if ((byte&0xE0)==0xE0) { /* 3-byte char copy */
        copylen = 3;
    } else if ((byte&0xC0)==0xC0) { /* 2-byte char copy */
        copylen = 2;
    } else if (byte <= 0x7F) { /* single char copy */
        copylen = 1;
    } else {
        printf("Uh-oh, we got a character we can't handle! Malformed UTF8?\n");
    }

    return copylen;
}

/* reverse a UTF-8 string without converting to Unicode code points */
char *reverse_utf8 (char *utf8buf)
{
    int len = strlen(utf8buf);
    int i, j = len-1;
    char *retbuf = malloc(len+1);

    /* for each byte of utf8buf, look at what range it's in and determine how
       many bytes to copy to output buffer -- place them accordingly */
    for (i = 0; i < len;) {
        int copylen = get_mbseq_len(&utf8buf[i]);
        if (copylen > 0) {
            memcpy(&retbuf[j-(copylen-1)], &utf8buf[i], copylen);
            i += copylen; j -= copylen;
        } else {
            free(retbuf);
            return NULL;
        }
    }

    retbuf[len] = '\0'; /* null terminate! */

    return retbuf;
}

int main (int argc, char *argv[])
{
    char *output;
    FILE *input;
    char buf[256];

    if (argc < 2) {
        printf("Usage: %s: [file]\n", argv[0]);
        return 1;
    }

    if ((input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    while (fgets(buf, 256, input)) {
        buf[strlen(buf)-1] = '\0'; /* chomp */
        printf("Original: %s\n", buf);
        output = reverse_utf8(buf);
        if (output == NULL) {
            printf("There was an error reversing this line.\n");
            continue;
        }
        printf("Reversed: %s\n", output);
        free(output);
    }

    return 0;
}
