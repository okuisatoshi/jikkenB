#include "scan.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int c;       // 最後に読んだ文字（あるいはEOF）
int row = 1; // 行番号(1から開始)
int col = 0; // 列番号(0から開始) colは文字の左端を指すので0開始が合理的

static int inc = 0; // 前回リターンした字句の長さ（文字数）

void initscan(void) {
    row = 1;
    col = 0;
    inc = 0;
    c = getchar();
}

static inline int kind(char *name) {
    if (strcmp("if", name) == 0) return TK_IF;
    if (strcmp("else", name) == 0) return TK_ELSE;
    if (strcmp("while", name) == 0) return TK_WHILE;
    if (strcmp("print", name) == 0) return TK_PRINT;
    if (strcmp("scan", name) == 0) return TK_SCAN;
    if (strcmp("int", name) == 0) return TK_INT;
    if (strcmp("return", name) == 0) return TK_RET;
    if (strcmp("break", name) == 0) return TK_BREAK;
    if (strcmp("continue", name) == 0) return TK_CONT;
    if (strcmp("putchar", name) == 0) return TK_PUTCHAR;
    if (strcmp("usleep", name) == 0) return TK_USLEEP;
    return TK_ID;
}

// 1文字(含:EOF)先読み状態で呼ばれ1文字(含:EOF)先読みした状態で返る
// その際，row,colはリターンされる字句の先頭（の文字の左）を，
// cは字句の末尾の次の文字（あるいはEOF）を指す
int get_next_token(char *name) {
    col += inc;
    inc = 0;
    // ここでは(row,col)は前回リターンした字句の末尾（の文字の右，すなわちcの左）
    // を指している開始時は(1,0)）
    // ここではcはその次の文字を指している
    // 空白のスキップ
    for (;;) {
        if (c == EOF) return 0;
        // コメント行(//...)のスキップ 演算子(/)の認識
        if (c == '/') {
            name[0] = '/';
            c = getchar();
            col++;
            if (c == '/') {
                c = getchar();
                col++;
                while (c != '\n') {
                    if (c == EOF) return 0;
                    c = getchar();
                    col++;
                }
            } else {
                name[1] = '\0';
                inc = 1;
                return '/';
            }
        }
        if (!isspace(c)) break;
        if (c == '\n') {
            row++;
            col = -1;
        } // 改行直後の文字の左が0になるように-1に
        c = getchar();
        col++;
    }
    // ここで!isspace(c)が成立 (cはEOFかもしれない)
    // ここでrow,colはcの左，すなわちリターンされる字句（またはコメント開始の//）の先頭を指している
    // これよりrow,colは不変，字句の長さはincでカウントする
    // 演算子(=, <, >, ==, <=, >=, >>)の認識
    if (c == '=' || c == '<' || c == '>') {
        name[0] = c;
        char first = c;
        c = getchar();
        inc++;
        if (c == '=') {
            name[1] = '=';
            name[2] = '\0';
            c = getchar();
            inc++;
            switch (first) {
            case '=':
                return TK_EQ;
            case '<':
                return TK_LEQ;
            case '>':
                return TK_GEQ;
            }
        }
        name[1] = '\0';
        return first;
    }
    // 演算子(!, !=)の認識
    if (c == '!') {
        name[0] = '!';
        c = getchar();
        inc++;
        if (c == '=') {
            name[1] = '=';
            name[2] = '\0';
            c = getchar();
            inc++;
            return TK_NEQ;
        }
        name[1] = '\0';
        return '!';
    }
    // 演算子(&, &&)の認識
    if (c == '&') {
        name[0] = '&';
        c = getchar();
        inc++;
        if (c == '&') {
            name[1] = '&';
            name[2] = '\0';
            c = getchar();
            inc++;
            return TK_AND;
        }
        name[1] = '\0';
        return '&';
    }
    // 演算子(|, ||)の認識
    if (c == '|') {
        name[0] = '|';
        c = getchar();
        inc++;
        if (c == '|') {
            name[1] = '|';
            name[2] = '\0';
            c = getchar();
            inc++;
            return TK_OR;
        }
        name[1] = '\0';
        return '|';
    }
    // 数(10進整数)[0-9]+の認識
    if (isdigit(c)) {
        name[0] = c;
        int i;
        for (i = 1; i < MAX_TK_LEN - 1; i++) {
            c = getchar();
            inc++;
            if (!isdigit(c)) break;
            name[i] = c;
        }
        name[i] = '\0';
        return TK_NUM;
    }
    // 識別子または2文字以上の予約語[a-zA-Z_][a-zA-Z0-9_]*の認識
    if (isalpha(c) || c == '_') {
        name[0] = c;
        int i;
        for (i = 1; i < MAX_TK_LEN - 1; i++) {
            c = getchar();
            inc++;
            if (!isalnum(c) && c != '_') break;
            name[i] = c;
        }
        name[i] = '\0';
        return kind(name);
    }
    // いずれでもない文字は1文字のトークンとして認識
    name[0] = c;
    name[1] = '\0';
    // 文字コードをトークン番号として使用
    int token = c;
    c = getchar();
    inc++;
    return token;
}
