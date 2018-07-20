#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ofstream fout;
    double a[100];
    fout.open("file.txt", ios::out|ios::binary);

    if (!fout) {
        cout << "ファイル file.txt が開けません";
        return 1;
    }
    for(int i=0;i<100;i++){
        a[i]=1.0*double(i);
        fout.write(( char * ) &a[i],sizeof( double ) );
    }
    fout.close();  //ファイルを閉じる
    return 0;
}
