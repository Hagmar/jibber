#include <stdio.h>

char data[5000] = {0};
char stack[5000] = {0};
char *funs[26] = {0};
char *p = data;
char *sp = stack;

void dump_stack() {
    char *sp = stack;
    while (*sp) printf("%d", (int)*sp++);
    printf("\n");
}

void interpret(char* program){
    char *c = program;
    char *fp;
    unsigned char i;

    while (*c != '.') {
        switch (*c) {
            case 'a':
                sp--;
                *(sp-1) += *sp;
                break;
            case 'b':
                if (! *--sp) {
                    while (*(++c) != ',');
                }
                break;
            case 'c':
                i = *(++c) - 'a';
                interpret(funs[i]);
                break;
            case 'd':
                (*(sp-1))--;
                break;
            case 'e':
                *sp = *(sp-1);
                sp++;
                break;
            case 'f':
                i = *(++c) - 'a';
                char f[1000];
                fp = f;
                funs[i] = f;
                while (*(++c) != '.') {
                    *(fp++) = *c;
                }
                *fp = '.';
                break;
            case 'i':
                (*(sp-1))++;
                break;
            case 'j':
                while (*c != ',' && *c != '.') c++;
                break;
            case 'l':
                while (*(--c) != ',');
                break;
            case 'm':
                sp--;
                *(sp-1) = *sp * *(sp-1);
                break;
            case 'n':
                p++;
                break;
            case 'o':
                printf("%c", *--sp);
                break;
            case 'p':
                p--;
                break;
            case 'r':
                *sp = getchar();
                while (*sp == '\n') *sp = getchar();
                sp++;
                break;
            case 's':
                sp--;
                *(sp-1) -= *sp;
                break;
            case 't':
                *sp++ = *++c;
                break;
            case 'w':
                *p = *--sp;
                break;
            case 'x':
                *sp++ = *p;
                break;
            case 'y':
                --sp;
                break;
            case 'z':
                *sp++ = 0;
                break;
            case ' ':
            case ',':
                break;
            default:
                fprintf(stderr, "Invalid command\n");
        }
        c++;
    }
}

int main(int argc, char** argv){
    if (argc < 2) {
        fprintf(stderr, "%s\n", "Please provide some code!");
    } else {
        interpret(argv[1]);
    }
    printf("\n");
    return 0;
}
