#include <bits/stdc++.h> //includes almost all standard libraries
using namespace std; //uses std automatically
struct process{
    string name;
    int arrival;
    int service;
};
struct algorithm{
    char id;
    int q = -1;
};


//global variables:

string operation; //trace or stats
vector<algorithm> algorithms;
vector<process> processes;
int last_instant;
int process_count;
//output timeline for trace
vector<vector<char>> timeline;
//output for stats
vector<int> finish_time;
vector<int> turn_around;
vector<int> norm_turn;

const string algorithm_names[8] = {"FSFC", "RR", "SPN", "SRT", "HRRN", "FB-1", 
"FB-2^i", "AGING"};

void parse_algorithms(string algorithm_line){
    stringstream stream(algorithm_line);
    string temp;
    while(getline(stream, temp, ',')){ 
        algorithm alg;   
        alg.id = temp[0];
        if (temp.size() > 2){
            temp = temp.substr(2);
            alg.q = stoi(temp);
        }
       algorithms.push_back(alg);
    }

}

void parse_processes(string process_line, int i){
    string process_name;
    int arrival_time;
    int service_time;
    stringstream stream(process_line);
    string temp;
    // getline(stream,temp,',');
    // process_name = temp;
    // getline(stream,temp,',');
    // arrival_time = stoi(temp);
    // getline(stream,temp,',');
    // service_time = stoi(temp);

    // cout << process_name << ", " << arrival_time << ", " << service_time << endl;

    getline(stream, temp, ',');
    process_name = temp;
    if (process_name.empty()) {
        cout << "Warning: Process name is empty!" << endl;
    }

    getline(stream, temp, ',');
    arrival_time = stoi(temp);
    if (arrival_time == 0 && temp != "0") {
        cout << "Warning: Arrival time might be incorrect!" << endl;
    }

    getline(stream, temp, ',');
    service_time = stoi(temp);
    if (service_time == 0 && temp != "0") {
        cout << "Warning: Service time might be incorrect!" << endl;
    }

    processes[i].name = process_name;
    processes[i].arrival = arrival_time;
    processes[i].service = service_time;

}

void parse(){
    //reading input
    string algorithms_line;
    cin>>operation>>algorithms_line>>last_instant>>process_count;
    //parsing processes
    string process_line;
    processes.resize(process_count);
    for(int i = 0;i < process_count;i++){
        cin >> process_line;
        parse_processes(process_line, i);
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


void print_timeline(){
    int alg_indx;
    string alg_name;

    for(int x = 0;x < (int)algorithms.size();x++){

        if (algorithms[x].q != -1){
            alg_indx = (algorithms[x].id - '0') -1; 
            alg_name = algorithm_names[alg_indx];
            cout << alg_name << "-";
            cout << left << setw(alg_name.size() + 2) << algorithms[x].q; 
        }else{
            alg_indx = (algorithms[x].id - '0') -1;
            alg_name = algorithm_names[alg_indx];
            cout << left << setw(3 + alg_name.size()) << alg_name;
        }

        for (int i = 0;i < last_instant;i++){
                cout << left << setw(2) << i%10;
            }

        cout << endl;

        for (int i = 0;i < last_instant + 6;i++){
            cout << "--";
        }
            
        cout << endl;
        
        for (int i = 0;i < process_count;i++){
            
            cout << left << setw(6) <<processes[i].name;
            
            for (int j = 0;j < last_instant;j++){
                cout << "|" << timeline[i][j];
            }

            cout << "|" << endl;
        
        }

        for (int i = 0;i < last_instant + 6;i++)
            cout << "--";
                
    }

}

void print_stats(){

}

int main(){
    parse();
    print_timeline();

    return 0;
}
