#include <bits/stdc++.h> //includes almost all standard libraries
using namespace std; //uses std automatically

//global variables:

string operation; //trace or stats
vector<tuple<char,int>> algorithms;
vector<tuple<string,int,int>> processes;
int last_instant;
int process_count;
//output timeline for trace
vector<vector<char>> timeline;
//output for stats
vector<int> finish_time;
vector<int> turn_around;
vector<int> norm_turn;

void parse_algorithms(string algorithm_line){
    stringstream stream(algorithm_line);
    char alg_id;
    int q;
    while(stream.good()){
        string temp;
        getline(stream,temp,',');    
        char alg_id = temp[0];

        stringstream stream2(temp);
        getline(stream2,temp,'-');
        getline(stream2,temp,'-');
        if(temp.size() > 0){
            q = stoi(temp);
        }else{
            q = -1;
        }
        algorithms.push_back(make_tuple(alg_id, q));
    }

}

void parse_processes(string process_line){
    string process_name;
    int arrival_time;
    int service_time;
    stringstream stream(process_line);
    string temp;
    getline(stream,temp,',');
    process_name = temp;
    getline(stream,temp,',');
    arrival_time = stoi(temp);
    getline(stream,temp,',');
    service_time = stoi(temp);

    processes.push_back(make_tuple(process_name, arrival_time, service_time));
}


void parse(){
    //reading input
    string algorithms_line;
    cin>>operation>>algorithms_line>>last_instant>>process_count;
    //parsing processes
    processes.resize(process_count);
    for(int i = 0;i < process_count;i++){
        string process_line;
        cin >> process_line;
        parse_processes(process_line);
    }
    //parsing algorithms
    parse_algorithms(algorithms_line);
    //adjusting stats vector sizes
    finish_time.resize(process_count);
    turn_around.resize(process_count);
    norm_turn.resize(process_count);
    timeline.resize(process_count);
    //adjusting trace timeline
    for(int i = 0;i < process_count;i++){
        for(int j = 0;j < last_instant;j++){
            timeline[i].push_back('.');
        }
    }
}


const string algorithm_names[8] = {"FSFC", "RR-", "SPN", "SRT", "HRRN", "FB-1", "FB-2^i", "AGING"};

// void print_timeline(){
//     for (int i = 0;i < algorithms.size();i++){
//         int idx = (get<0>(algorithms[i])) - '0';
//         cout << algorithm_names[idx] << "  ";
//         for (int i = 0;i < last_instant;i++)
//             cout << i%10;
//         cout << endl;
//         for (int i = 0;i < last_instant;i++)
//             cout << "--";
//         for(int i = 0;i < process_count;i++)
//             for(int j = 0;j < last_instant;j++)
//                 cout << timeline[i][j];
//     }
// }

int main(){
    parse();

    return 0;
}
