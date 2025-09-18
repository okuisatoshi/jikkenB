/*
 * Pico-Cコンパイラ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scan.h"
#include "hashmap.h"

int token;             // 最後に読んだトークン
char name[MAX_TK_LEN]; // ...の名前
int vsuffix = 1;       // 値の一時名の接尾番号
int lsuffix = 1;       // ラベルの接尾番号
struct hashmap *hmap;  // 変数名と行番号の対を保存するハッシュ表
#define MAX_TMP_LEN (MAX_TK_LEN + 1) // 値の一時名の最大長

// デバッグモードでは標準エラー出力に構文解析木が出力される
//#define DEBUG
int findent = 0;
#ifdef DEBUG
#define ENTER() { findent += 3; fprintf(stderr, "%*c|- %s\n", findent, ' ', __func__); }
#define LEAVE() findent -= 3
#define FEED() fprintf(stderr, "%*c   \'\e[7m%s\e[0m\' \e[2m (行%d:%d)\e[0m\n", findent, ' ', name, row, col)
#else
#define ENTER()
#define LEAVE()
#define FEED()
#endif

void statement(void);
void expr(char *ret);


void expect(int expected)
{
    if (token == expected) {
        FEED();
        token = get_next_token(name);
        return;
    }
    fprintf(stderr, "期待されない字句(行%d:%d): %s\n", row, col, name);
    exit(1);
}


void lval(char *x)
{
    ENTER();
    if (hget(hmap, name) < 0) {
        fprintf(stderr, "不明な変数(行%d:%d): %s\n", row, col, name);
        exit(1);
    }
    sprintf(x, "%%%s", name);
    expect(TK_ID);
    LEAVE();
}


void num(char *ret)
{
    ENTER();
    strcpy(ret, name);
    expect(TK_NUM);
    LEAVE();
}


void factor(char *ret)
{
    ENTER();
    char x[MAX_TMP_LEN];
    int v = vsuffix++;
    switch (token) {
        case TK_ID:
            lval(x);
            // 代入の左辺以外に出現する左辺値は参照を外す
            printf("  %%t.%d = load i32, ptr %s\n", v, x);
            sprintf(ret, "%%t.%d", v);
            break;
        case TK_NUM:
            num(ret);
            break;
        case  '(':
            expect('(');
            expr(ret);
            expect(')');
            break;
        default:
            fprintf(stderr, "期待されない字句(行%d:%d): %s\n", row, col, name);
            exit(1);
    }
    LEAVE();
}


void term(char *left)
{
    ENTER();
    factor(left);
    while (token == '*' || token == '/' || token == '%') {
        int op = token;
        expect(op);
        char right[MAX_TMP_LEN];
        factor(right);
        int v = vsuffix++;
        const char *f = op == '*' ? "mul" : op == '/' ? "sdiv" : "srem";
        printf("  %%t.%d = %s i32 %s, %s\n", v, f, left, right);
        sprintf(left, "%%t.%d", v);
    }
    LEAVE();
}


void additive(char *left)
{
    ENTER();
    term(left);
    while (token == '+' || token == '-') {
        int op = token;
        expect(op);
        char right[MAX_TMP_LEN];
        term(right);
        int v = vsuffix++;
        const char *f = op == '+' ? "add" : "sub";
        printf("  %%t.%d = %s i32 %s, %s\n", v, f, left, right);
        sprintf(left, "%%t.%d", v);
    }
    LEAVE();
}


void expr(char *left)
{
    ENTER();
    additive(left);
    while (token == '<' || token == '>' || token == TK_LEQ || token == TK_GEQ) {
        int op = token;
        expect(op);
        char right[MAX_TMP_LEN];
        additive(right);
        int u = vsuffix++, v = vsuffix++;
        const char *f = op == '<' ? "slt" : op == '>' ? "sgt" : op == TK_LEQ ? "sle" : "sge";
        printf("  %%t.%d = icmp %s i32 %s, %s\n", u, f, left, right);
        printf("  %%t.%d = zext i1 %%t.%d to i32\n", v, u);
        sprintf(left, "%%t.%d", v);
    }
    LEAVE();
}


void print_st(void)
{
    ENTER();
    expect(TK_PRINT);
    char e[MAX_TMP_LEN];
    expr(e);
    printf("  %%t.%d = call i32 (ptr, ...) @printf(ptr @fmt_printf, i32 %s)\n", vsuffix++, e);
    expect(';');
    LEAVE();
}


void scan_st(void)
{
    ENTER();
    expect(TK_SCAN);
    char x[MAX_TMP_LEN];
    lval(x);
    printf("  %%t.%d = call i32 (ptr, ...) @scanf(ptr @fmt_scanf, ptr %s)\n", vsuffix++, x);
    expect(';');
    LEAVE();
}


void assign_st(void)
{
    ENTER();
    char x[MAX_TMP_LEN];
    lval(x);
    expect('=');
    char e[MAX_TMP_LEN];
    expr(e);
    printf("  store i32 %s, ptr %s\n", e, x);
    expect(';');
    LEAVE();
}


void while_st(void)
{
    ENTER();
    int while_entry = lsuffix++;
    int while_body = lsuffix++;
    int while_end = lsuffix++;
    printf("  br label %%L.%d\n", while_entry);

    // whileループ冒頭のブロック
    printf("L.%d:\n", while_entry);
    expect(TK_WHILE);
    expect('(');
    char e[MAX_TMP_LEN];
    expr(e);
    expect(')');
    int c = vsuffix++;
    printf("  %%t.%d = icmp eq i32 %s, 0\n", c, e);
    printf("  br i1 %%t.%d, label %%L.%d, label %%L.%d\n", c, while_end, while_body);

    // whileループ本体のブロック
    printf("L.%d:\n", while_body);
    statement();
    printf("  br label %%L.%d\n", while_entry);

    // whileループの直後のブロック
    printf("L.%d:\n", while_end);
    LEAVE();
}


void decl(void)
{
    ENTER();
    expect(TK_INT);
    int r = hget(hmap, name);
    if (r >= 0) {
        fprintf(stderr, "宣言の重複(行%d:%d): %s (行%dで既出)\n", row, col, name, r);
        exit(1);
    }
    char *x = malloc(MAX_TK_LEN); // hdestroy()内でfree()される
    strcpy(x, name);
    hinsert(hmap, x, row); // xの宣言の行番号を記録
    expect(TK_ID);
    printf("  %%%s = alloca i32\n", x);
    expect(';');
    LEAVE();
}


void block(void)
{
    ENTER();
    expect('{');
    while (token != '}') {
        if (token == TK_INT) decl(); else statement();
    }
    expect('}');
    LEAVE();
}


void statement(void)
{
    ENTER();
    switch (token) {
        case TK_PRINT:
            print_st();
            break;
        case TK_SCAN:
            scan_st();
            break;
        case TK_ID:
            assign_st();
            break;
        case TK_WHILE:
            while_st();
            break;
        case '{':
            block();
            break;
        default:
            fprintf(stderr, "期待されない字句(行%d:%d): %s\n", row, col, name);
            exit(1);
    }
    LEAVE();
}


void program(void)
{
    ENTER();
    printf("@fmt_printf = constant [4 x i8] c\"%%d\\0A\\00\"\n");
    printf("@fmt_scanf  = constant [3 x i8] c\"%%d\\00\"\n");
    printf("declare i32 @printf(ptr, ...)\n");
    printf("declare i32 @scanf(ptr, ...)\n");
    printf("@__main_void = alias i32 (), ptr @main\n\n"); // target:wasm32-wasiで必要

    expect(TK_INT);
    if (strcmp(name, "main") != 0) {
        fprintf(stderr, "main関数が必要(行%d:%d): %s\n", row, col, name);
        exit(1);
    }
    expect(TK_ID);
    expect('(');
    expect(')');
    printf("define i32 @main() {\n");
    block();
    printf("  ret i32 0\n");
    printf("}\n");
    LEAVE();
}


int main()
{
    hmap = hnew();
    token = get_next_token(name);
    program();
    hdestroy(hmap); // キーもここでfreeしている
}
