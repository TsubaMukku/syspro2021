#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(void){

    char c;
    int fps, fpd;
    int count;
    
    /*
        openの引数は
            オープンしたいファイル名
            オープンの方法を指示するflag

            O_RDONLY: 読み込みだけ
    */
    struct timespec ts;
    struct tm tm;

    clock_gettime(CLOCK_REALTIME, &ts);
    localtime_r( &ts.tv_sec, &tm);
    printf("%d/%02d/%02d %02d:%02d:%02d.%09ld\n",
		tm.tm_year+1900,
		tm.tm_mon+1,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		ts.tv_nsec);
    fps = open("sample1.txt", O_RDONLY);
    
    if (fps < 0) {
        /*
            perror関数によりエラーメッセージを出力する。
            引数は出力したいメッセージを与える。
        */
        perror("open: src");
        exit(1); // プログラム実行を終了する
    }
    
    /*
        オープンしたいファイル名はdstである
        O_WRONLY は書き込み専用を意味する
        O_CREATがセットされている場合、ファイルがない時にファイルを作る
        O_TRUNCがセットされているとファイルが存在する場合にファイルの中身が消されてファイル長0となる。
    */
    fpd = open("dst", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // fpd = -1の場合、ファイル読み込みに失敗している
    if (fpd < 0) {
        perror("open: dst");
        close(fps);
        exit(1);
    }

    /*
        read:低水準出力言語である（ソースから指定メモリ領域にコピーする）
        int read(
            int fps: ファイルのハンドル
            void *buf: データ格納先のアドレスを表す
            int length: bufパラメータの指すバッファのバイト単位長
        )
    */
   
   // count:読み込みに成功すると受信データのバイト数を返す
   // count > 0とはデータを読み込めることを意味する
    while ((count = read(fps, &c, 1)) > 0) {

        /*
        int write(
            int fpd: ファイルハンドルを表す
            void *buf: データアドレスを指す
            
        )
        */
         if (write(fpd, &c, count) < 0) {
             perror("write");
             close(fpd);
             close(fps);
            exit(1);
        }
    }
    
    if (count < 0) {
        perror("read");
		close(fpd);
		close(fps);
        exit(1);
    }
    
    // close:ファイルを閉じる低水準入出力関数である
    close(fpd);
    close(fps);

    struct timespec ts2;
    struct tm tm2;

    clock_gettime(CLOCK_REALTIME, &ts2);
    localtime_r( &ts2.tv_sec, &tm2);
    printf("%d/%02d/%02d %02d:%02d:%02d.%09ld\n",
		tm2.tm_year+1900,
		tm2.tm_mon+1,
		tm2.tm_mday,
		tm2.tm_hour,
		tm2.tm_min,
		tm2.tm_sec,
		ts2.tv_nsec);
    return 0;
}