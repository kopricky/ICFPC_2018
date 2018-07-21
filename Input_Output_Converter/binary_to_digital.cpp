//charで受け取っているためbinaryの長さがバイト単位ではなく躊躇半端に切れている場合返り値の最後の数桁に余分な0が加わる

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#define ll long long

using namespace std;

int mdl_to_txt()
{
    char input_file_name[] = "./sample_input.mdl";
    char output_file_name[] = "./sample_output.txt";

    ifstream fin(input_file_name, ios::in | ios::binary );
    ofstream fout(output_file_name, ios::out);

    if(!fin || !fout){
        cout << "ファイルが開けません" << "\n";
        return 1;
    }

    while(!fin.eof()) {
        char buff;
        fin.read(&buff,sizeof(char));
        for(int i = 0; i < 8; i++){
            fout << ((buff >> i)&1);
        }
    }

    cout << "\n";

    return 0;
}

int main()
{
    assert(!mdl_to_txt());
    return 0;
}
