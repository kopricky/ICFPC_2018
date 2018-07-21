#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>
#define ll long long

using namespace std;

vector<int> convert_to_zero_one(int num, int digit)
{
    vector<int> res(digit, 0);
    for(int i = 0; i < digit; i++){
        res[i] = num % 2;
        num /= 2;
    }
    return res;
}

pair<vector<int>, vector<int> > short_move(string dir, int dist)
{
    int a;
    if(dir == "x"){
        a = 1;
    }else if(dir =="y"){
        a = 2;
    }else if(dir == "z"){
        a = 3;
    }else{
        assert(false);
    }
    return make_pair(convert_to_zero_one(a, 2), convert_to_zero_one(dist+5, 4));
}

pair<vector<int>, vector<int> > long_move(string dir, int dist)
{
    int a;
    if(dir == "x"){
        a = 1;
    }else if(dir =="y"){
        a = 2;
    }else if(dir == "z"){
        a = 3;
    }else{
        assert(false);
    }
    return make_pair(convert_to_zero_one(a, 2), convert_to_zero_one(dist+15, 5));
}

vector<int> near_move(int dx, int dy, int dz)
{
    return convert_to_zero_one((dx+1)*9+(dy+1)*3+(dz+1), 5);
}

int txt_to_nbt()
{
    char input_file_name[] = "./input.txt";
    char output_file_name[] = "./output.nbt";

    ifstream fin(input_file_name, ios::in);
    ofstream fout(output_file_name, ios::out | ios::binary);

    if(!fin || !fout){
        cout << "ファイルが開けません" << "\n";
        return 1;
    }

    string s;
    while(fin >> s){
        if(s == "Halt"){
            int num = (1 << 8) - 1;
            fout.write((char*) &num, sizeof(char));
            return 0;
        }else if(s == "Wait"){
            int num = (1 << 8) - 2;
            fout.write((char*) &num, sizeof(char));
        }else if(s == "Flip"){
            int num = (1 << 8) - 3;
            fout.write((char*) &num, sizeof(char));
        }else if(s == "SMove"){
            string dir;
            int dist;
            fin >> dir >> dist;
            int num1 = (1 << 2), num2 = 0;
            auto p = long_move(dir, dist);
            for(int i = 0; i < (int)(p.first).size(); i++){
                num1 += (p.first[i] << (i+4));
            }
            for(int i = 0; i < (int)(p.second).size(); i++){
                num2 += (p.second[i] << i);
            }
            fout.write((char*) &num1, sizeof(char));
            fout.write((char*) &num2, sizeof(char));
        }else if(s == "LMove"){
            string dir1, dir2;
            int dist1, dist2;
            fin >> dir1 >> dist1 >> dir2 >> dist2;
            auto p1 = short_move(dir1, dist1);
            auto p2 = short_move(dir2, dist2);
            int num1 = (1 << 2) + (1 << 3), num2 = 0;
            for(int i = 0; i < (int)(p1.first).size(); i++){
                num1 += (p1.first[i] << (i+4));
            }
            for(int i = 0; i < (int)(p2.first).size(); i++){
                num1 += (p2.first[i] << (i+6));
            }
            for(int i = 0; i < (int)(p1.second).size(); i++){
                num2 += (p1.second[i] << i);
            }
            for(int i = 0; i < (int)(p2.second).size(); i++){
                num2 += (p2.second[i] << (i+4));
            }
            fout.write((char*) &num1, sizeof(char));
            fout.write((char*) &num2, sizeof(char));
        }else if(s == "Fission"){
            int dx, dy, dz, m;
            fin >> dx >> dy >> dz >> m;
            int num1 = 1 + (1 << 2), num2 = 0;
            auto p1 = near_move(dx, dy, dz);
            for(int i = 0; i < (int)p1.size(); i++){
                num1 += (p1[i] << (i+3));
            }
            auto p2 = convert_to_zero_one(m, 8);
            for(int i = 0; i < (int)p2.size(); i++){
                num2 += (p2[i] << i);
            }
            fout.write((char*) &num1, sizeof(char));
            fout.write((char*) &num2, sizeof(char));
        }else if(s == "Fill"){
            int dx, dy, dz;
            fin >> dx >> dy >> dz;
            int num = 3;
            auto p = near_move(dx, dy, dz);
            for(int i = 0; i < (int)p.size(); i++){
                num += (p[i] << (i+3));
            }
            fout.write((char*) &num, sizeof(char));
        }else if(s == "FusionP"){
            int dx, dy, dz;
            fin >> dx >> dy >> dz;
            int num = 7;
            auto p = near_move(dx, dy, dz);
            for(int i = 0; i < (int)p.size(); i++){
                num += (p[i] << (i+3));
            }
            fout.write((char*) &num, sizeof(char));
        }else if(s == "FusionS"){
            int dx, dy, dz;
            fin >> dx >> dy >> dz;
            int num = (1 << 1) + (1 << 2);
            auto p = near_move(dx, dy, dz);
            for(int i = 0; i < (int)p.size(); i++){
                num += (p[i] << (i+3));
            }
            fout.write((char*) &num, sizeof(char));
        }else{
            return 1;
        }
    }
    return 1;
}

int main()
{
    assert(!txt_to_nbt());
    return 0;
}
