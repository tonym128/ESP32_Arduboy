// 参照ファイルのインクルード
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// メインプログラムのエントリ
//
int main(int argc, const char *argv[])
{
    // 入力ファイル名の初期化
    const char *inname = NULL;
    
    // 出力ファイル名の初期化
    const char *outname = NULL;

    // 出力ファイルサイズの初期化
    int outsize = 0;

    // 名前の初期化
    const char *name = "patternTable";
    
    // 引数の取得
    while (--argc > 0) {
        ++argv;
        if (strcasecmp(*argv, "-o") == 0) {
            outname = *++argv;
            --argc;
        } else if (strcasecmp(*argv, "-s") == 0) {
            outsize = atoi(*++argv);
            --argc;
        } else if (strcasecmp(*argv, "-n") == 0) {
            name = *++argv;
            --argc;
        } else {
            inname = *argv;
        }
    }

    // 入力ファイルがない
    if (inname == NULL) {
        return -1;
    }

    // 出力ファイルがない
    if (outname == NULL) {
        outname = inname;
    }
    
    // パスの取得
    char dirname[FILENAME_MAX];
    char basename[FILENAME_MAX];
    char extname[FILENAME_MAX];
    {
        dirname[0] = '\0';
        basename[0] = '\0';
        extname[0] = '\0';
    }
    {
        int length = strlen(outname);
        int i = length - 1;
        while (i > 0 && outname[i] != '/' && outname[i] != '.') {
            --i;
        }
        if (i >= 0 && outname[i] == '.') {
            int n = length - (i + 1);
            strncpy(extname, &outname[i + 1], n);
            extname[n] = '\0';
            length = i;
        }
        i = length - 1;
        while (i > 0 && outname[i] != '/') {
            --i;
        }
        if (i >= 0) {
            if (outname[i] == '/') {
                int n = length - (i + 1);
                strncpy(basename, &outname[i + 1], n);
                basename[n] = '\0';
                length = i;
            } else {
                strncpy(basename, &outname[i], length);
                basename[length] = '\0';
                length = i;
            }
        }
        i = length - 1;
        if (length > 0) {
            strncpy(dirname, outname, length);
            dirname[length] = '\0';
        }
    }

    // .cpp ファイル名の取得
    char cppname[FILENAME_MAX];
    cppname[0] = '\0';
    if (dirname[0] != '\0') {
        strcat(cppname, dirname);
        strcat(cppname, "/");
    }
    strcat(cppname, basename);
    strcat(cppname, ".cpp");

    // .h ファイル名の取得
    char hname[FILENAME_MAX];
    hname[0] = '\0';
    if (dirname[0] != '\0') {
        strcat(hname, dirname);
        strcat(hname, "/");
    }
    strcat(hname, basename);
    strcat(hname, ".h");

    // 入力ファイルを開く
    FILE *infile = fopen(inname, "rb");
    
    // ファイルサイズの取得
    if (outsize == 0) {
        fseek(infile, 0, SEEK_END);
        outsize = ftell(infile);
        fseek(infile, 0, SEEK_SET);
    }
    outsize = outsize & ~7;
    
    // .cpp ファイルを開く
    FILE *cppfile = fopen(cppname, "w");
    
    // ヘッダの出力
    fprintf(cppfile, "// %s\n//\n", inname);

    // 参照ファイルの出力
    fprintf(cppfile, "#include \"%s.h\"\n\n", basename);
    
    // 配列の定義の出力
    fprintf(cppfile, "const unsigned char %s[]  = {\n", name);
    
    // 配列のデータの出力
    int insize = 0;
    int data[8];
    bool end = false;
    while (!end) {
        for (int i = 0; i < 8; i++) {
            if ((data[i] = fgetc(infile)) == EOF || insize >= outsize) {
                end = true;
                break;
            }
            ++insize;
        }
        if (!end) {
            fprintf(cppfile, "    ");
            for (int i = 0; i < 8; i++) {
                int c = 0x00;
                for (int j = 0; j < 8; j++) {
                    c <<= 1;
                    c |= (data[7 - j] >> (7 - i)) & 0x01;
                }
                fprintf(cppfile, "0x%02x, ", c);
            }
            fprintf(cppfile, "\n");
        }
    }
    
    // 配列の終端の出力
    fprintf(cppfile, "};\n\n");
    
    // .cpp ファイルを閉じる
    if (cppfile != stdout) {
        fclose(cppfile);
    }
    
    // 入力ファイルを閉じる
    fclose(infile);

    // 定義名の取得
    char defname[FILENAME_MAX];
    {
        char *def = defname;
        *def++ = '_';
        {
            char *base = basename;
            while (*base != '\0') {
                *def++ = toupper(*base++);
            }
        }
        strcpy(def, "_H_");
    }

    // .h ファイルを開く
    FILE *hfile = fopen(hname, "w");

    // ヘッダの出力
    fprintf(hfile, "// %s\n//\n", inname);
    fprintf(hfile, "#ifndef %s\n", defname);
    fprintf(hfile, "#define %s\n\n", defname);

    // 参照ファイルの出力
    fprintf(hfile, "#include <pgmspace.h>\n\n");

    // 宣言の出力
    fprintf(hfile, "extern const unsigned char %s[];\n\n", name);

    // フッタの出力
    fprintf(hfile, "#endif\n");

    // .h ファイルを閉じる
    if (hfile != stdout) {
        fclose(hfile);
    }
    
    // 終了
    return 0;
}


