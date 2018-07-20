#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#define ll long long

using namespace std;

const int MAX_SIZE = 20000000;

bool bit[MAX_SIZE];

struct coordinate
{
    int x,y,z;
    coordinate(){}
    coordinate(int x_, int y_, int z_) : x(x_), y(y_), z(z_){}
};

int txt_to_mdl()
{
    char input_file_name[] = "./sample_output.txt";
    char output_file_name[] = "./sample_input.mdl";

    ifstream fin(input_file_name, ios::in);
    ofstream fout(output_file_name, ios::out | ios::binary);

    if(!fin || !fout){
        cout << "ファイルが開けません" << "\n";
        return 1;
    }

    int R;
    fin >> R;

    int cube = R * R * R;

    int n;

    fin >> n;

    for(int i = 0; i < n; i++){
        int x,y,z;
        fin >> x >> y >> z;
        bit[x*R*R+y*R+z] = true;
    }

    fout.write((char*) &R, sizeof(char));

    int index = 0;

    while(1){
        if(index+8 > cube){
            int num = 0;
            for(int i = 0; i < cube-index; i++){
                num += (bit[index+i] << i);
            }
            fout.write((char*) &num, cube-index);
            break;
        }
        int num = 0;
        for(int i = 0; i < 8; i++){
            num += (bit[index+i] << i);
        }
        fout.write((char*) &num, sizeof(char));
        index += 8;
    }

    fin.close();
    fout.close();

    return 0;
}

int main()
{
    assert(!txt_to_mdl());
    return 0;
}
