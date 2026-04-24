int printf(const char *fmt, ...);
// EXPECT: Token parser:\nNUM:123 OP:+ NUM:45 OP:* NUM:2 OP:- NUM:10 \nBracket matcher:\nvalid=1 max_depth=2\nWord counter:\nwords=3\nRun-length encode:\na3b2c4a1
int main(void) {
    char input[30];
    input[0] = '1'; input[1] = '2'; input[2] = '3';
    input[3] = '+'; input[4] = '4'; input[5] = '5';
    input[6] = '*'; input[7] = '2';
    input[8] = '-'; input[9] = '1'; input[10] = '0';
    input[11] = 0;

    printf("Token parser:\n");
    int state = 0;
    int num = 0;
    int tokens[20];
    int types[20];
    int ntokens = 0;
    int i = 0;

    while (input[i] != 0) {
        char c = input[i];
        switch (state) {
            case 0:
                if (c >= '0' && c <= '9') {
                    num = c - '0';
                    state = 1;
                } else {
                    types[ntokens] = 1;
                    tokens[ntokens] = c;
                    ntokens++;
                }
                break;
            case 1:
                if (c >= '0' && c <= '9') {
                    num = num * 10 + (c - '0');
                } else {
                    types[ntokens] = 0;
                    tokens[ntokens] = num;
                    ntokens++;
                    types[ntokens] = 1;
                    tokens[ntokens] = c;
                    ntokens++;
                    state = 0;
                }
                break;
        }
        i++;
    }
    if (state == 1) {
        types[ntokens] = 0;
        tokens[ntokens] = num;
        ntokens++;
    }

    for (i = 0; i < ntokens; i++) {
        if (types[i] == 0) {
            printf("NUM:%d ", tokens[i]);
        } else {
            printf("OP:%c ", tokens[i]);
        }
    }
    printf("\n");

    printf("Bracket matcher:\n");
    char expr[20];
    expr[0] = '('; expr[1] = '('; expr[2] = ')';
    expr[3] = '('; expr[4] = ')'; expr[5] = ')';
    expr[6] = 0;
    int depth = 0;
    int max_depth = 0;
    int valid = 1;
    i = 0;
    while (expr[i] != 0) {
        if (expr[i] == '(') {
            depth++;
            if (depth > max_depth) max_depth = depth;
        } else if (expr[i] == ')') {
            depth--;
            if (depth < 0) {
                valid = 0;
                break;
            }
        }
        i++;
    }
    if (depth != 0) valid = 0;
    printf("valid=%d max_depth=%d\n", valid, max_depth);

    printf("Word counter:\n");
    char text[40];
    text[0] = 'h'; text[1] = 'i'; text[2] = ' ';
    text[3] = ' '; text[4] = 't'; text[5] = 'h';
    text[6] = 'e'; text[7] = 'r'; text[8] = 'e';
    text[9] = ' '; text[10] = 'w'; text[11] = 'o';
    text[12] = 'r'; text[13] = 'l'; text[14] = 'd';
    text[15] = 0;
    int words = 0;
    int in_word = 0;
    i = 0;
    while (text[i] != 0) {
        switch (in_word) {
            case 0:
                if (text[i] != ' ') {
                    in_word = 1;
                    words++;
                }
                break;
            case 1:
                if (text[i] == ' ') {
                    in_word = 0;
                }
                break;
        }
        i++;
    }
    printf("words=%d\n", words);

    printf("Run-length encode:\n");
    char rle[12];
    rle[0] = 'a'; rle[1] = 'a'; rle[2] = 'a';
    rle[3] = 'b'; rle[4] = 'b';
    rle[5] = 'c';
    rle[6] = 'c'; rle[7] = 'c'; rle[8] = 'c';
    rle[9] = 'a';
    rle[10] = 0;
    char cur = rle[0];
    int count = 1;
    i = 1;
    while (rle[i] != 0) {
        if (rle[i] == cur) {
            count++;
        } else {
            printf("%c%d", cur, count);
            cur = rle[i];
            count = 1;
        }
        i++;
    }
    printf("%c%d\n", cur, count);

    return 0;
}
