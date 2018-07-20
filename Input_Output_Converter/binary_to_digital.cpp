#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#define ll long long

using namespace std;

struct coordinate
{
    int x,y,z;
    coordinate(){}
    coordinate(int x_, int y_, int z_) : x(x_), y(y_), z(z_){}
};

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

    char buff;

    fin.read(&buff, sizeof(char));

    int R = 0;

    for(int i = 0; i < 8; i++){
        if((buff >> i)&1) R += (1 << i);
    }

    fout << R << "\n";

    int cube = R * R * R;

    int index = 0;

    vector<coordinate> block;

    while(!fin.eof()) {
        char buff;
        fin.read(&buff,sizeof(char));
        for(int i = 0; i < 8 && index < cube; i++, index++){
            if((buff >> i)&1){
                block.emplace_back(index / R / R, (index / R) % R, index % R);
            }
        }
    }

    fout << (int)block.size() << "\n";

    for(auto& val : block){
        fout << val.x << " " << val.y << " " << val.z << "\n";
    }

    fin.close();
    fout.close();

    return 0;
}

int main()
{
    assert(!mdl_to_txt());
    return 0;
}
