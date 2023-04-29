#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char const *argv[])
{
    ifstream fin;
    ofstream fout1;
    ofstream fout2;
    string line;
    vector<double> open_values;
    vector<double> close_values;

    fin.open("lastest_data.csv");    
    fout1.open("data.csv");
    fout2.open("display.csv");
    if(!fin.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }    
    while(getline(fin, line))
    {
        stringstream ss(line);
        string token;
        int count = 0;
        while(getline(ss, token, ','))
        {
            if(count == 2)
            {
                open_values.push_back(stof(token));
            }
            else if(count == 3)
            {
                close_values.push_back(stof(token));
            }
            count++;
        }
    }
    fin.close();
    cout << open_values.size() << " " << close_values.size() << endl;
    for(int i = 0; i < open_values.size(); i++)
    {
        fout1 << open_values[i] - close_values[i] << endl;
        fout2 << i << ", " <<  open_values[i] - close_values[i] << endl;
    }
    fout1.close();
    fout2.close();

    // Find max and min
    double max = open_values[0] - close_values[0];
    double min = open_values[0] - close_values[0];
    for(int i = 0; i < open_values.size(); i++)
    {
        if(open_values[i] - close_values[i] > max)
        {
            max = open_values[i] - close_values[i];
        }
        if(open_values[i] - close_values[i] < min)
        {
            min = open_values[i] - close_values[i];
        }
    }
    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl;
    
    return 0;
}
