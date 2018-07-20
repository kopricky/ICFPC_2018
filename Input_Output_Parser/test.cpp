#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#define ll long long
#define show(x) cout<<#x<<" = "<<(x)<<endl

using namespace std;

const int MAX_SIZE = 2000000;

bool bit[MAX_SIZE];

struct coordinate
{
    int x,y,z;
    coordinate(){}
    coordinate(int x_, int y_, int z_) : x(x_), y(y_), z(z_){}
};

int read_input()
{
    char input_file_name[] = "./sample_input.mdl";
    char output_file_name[] = "./sample_output.txt";

    ifstream fin(input_file_name, ios::in | ios::binary );
    ofstream fout(output_file_name, ios::out);

    if(!fin || !fout){
        cout << "ファイルが開けません" << "\n";
        return 1;
    }

    char* buff = new char[MAX_SIZE];

    fin.read(buff, sizeof(buff));

    int R = 0;

    int index = 0;

    vector<coordinate> block;

    fin.read((char*)bit,sizeof(bool)*MAX_SIZE);

    for(;index < 8; index++){
        R += (1 << bit[7-index]);
    }

    fout << R << "\n";

    int cube = R * R * R;

    for(;index < cube; index++){
        if(bit[index]){
            int val = index - 8;
            block.emplace_back(val / R / R, (val / R) % R, val % R);
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
    assert(!read_input());
    return 0;
}
