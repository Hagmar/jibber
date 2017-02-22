#include <stdio.h>

char data[5000] = {0};
char stack[5000] = {0};
char *funs[256] = {0};
char *p = data;
char *sp = stack;

void dump_stack() {
    char *sp = stack;
    while (*sp) printf("%d", (int)*sp++);
    printf("\n");
}

void interpret(char* program){
    char *c = program;
    unsigned char i;

    while (*c != '.') {
        switch (*c) {
            case 'a':
                sp--;
                *(sp-1) += *sp;
                break;
            case 'b':
                if (! *--sp) {
                    i = 0;
                    unsigned char skip = 0;
                    while (*++c != ',' || i || skip){
                        if (skip) {
                            skip = 0;
                        } else {
                            switch (*c) {
                                case 'c':
                                case 'f':
                                case 't':
                                    skip = 1;
                                    break;
                                case 'b':
                                case 'j':
                                    i++;
                                    break;
                                case ',':
                                    i--;
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;
            case 'c':
                i = *(++c);
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
                i = *(++c);
                funs[i] = c+1;
                while (*(++c) != '.') ;
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
