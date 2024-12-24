#include <bits/stdc++.h> //includes almost all standard libraries
#include <format>
using namespace std; //uses std automatically
struct process{
    string name;
    int arrival;
    int service;
    bool done = false;
};
struct algorithm{
    char id; // 1->8
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
vector<float> norm_turn;

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

//resets the done variable of all processes to false
void reset_processes(){
    for (int i=0;i < process_count;i++){
        processes[i].done = false;
    }
}

//NOT DONE !!!!
void sort_processes(){
    
}

void print_timeline(int x){
    int alg_indx;
    string alg_name;

    if (algorithms[x].q != -1){
        alg_indx = (algorithms[x].id - '0') -1; 
        alg_name = algorithm_names[alg_indx];
        cout << alg_name << "-";
        cout << left << setw(2 + (int)alg_name.size()) << algorithms[x].q; 
    }else{
        alg_indx = (algorithms[x].id - '0') -1;
        alg_name = algorithm_names[alg_indx];
        cout << left << setw(7) << alg_name;
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
        
        cout << left << setw(7 - processes[i].name.size()) <<processes[i].name;
        
        for (int j = 0;j < last_instant;j++){
            cout << "|" << timeline[i][j];
        }

        cout << "|" << endl;
    
    }

    for (int i = 0;i < last_instant + 6;i++)
        cout << "--";

}

//NOT DONE !!!!!
void print_stats(int x){
    cout << endl;
    cout << algorithm_names[x] << endl;

    cout << left << setw(11) << "Process";
    cout << "|";
    for (int i = 0;i < process_count;i++){
        int space = ceil(7 - processes[i].name.size()) / 2;
        cout << right << setw(space) << processes[i].name << right << setw(space) << "|" ;
    }

    cout << endl;
    cout << left << setw(11) << "Arrival";
    cout << "|";
    for (int i = 0;i < process_count;i++){
        int space = ceil(7 - to_string(processes[i].arrival).size()) / 2;
        cout << right << setw(space) << processes[i].arrival << right << setw(space) << "|" ;
    }

    cout << endl;
    cout << left << setw(11) << "Service";
    cout << "|";
    for (int i = 0;i < process_count;i++){
        int space = ceil(7 - to_string(processes[i].service).size()) / 2;
        cout << right << setw(space) << processes[i].service << right << setw(space) << "|" ;
    }
    cout << " Mean" << "|";

    cout << endl;
    cout << left << setw(11) << "Finish";
    cout << "|";
    for (int i = 0;i < process_count;i++){
        int space = ceil((7 - to_string(finish_time[i]).size())) / 2;
        cout << right << setw(space) << finish_time[i] << right << setw(space) << "|" ;
    }
    cout << "-----" << "|";

    cout << endl;
    float mean = 0.00;
    float sum = 0.00;
    cout << left << setw(11) << "Turnaround";
    cout << "|";
    for (int i = 0;i < process_count;i++){
        sum += turn_around[i];
        int space = ceil(7 - (float)to_string(turn_around[i]).size()) / 2;
        cout << right << setw(space) << turn_around[i] << right << setw(space) << "|" ;
    }
    mean = sum / process_count;
    cout << right << setw(5) << fixed << setprecision(2) << mean << "|";

    cout << endl;
    mean = 0.00;
    sum = 0.00;
    cout << left << setw(11) << "NormTurn"; 
    cout << "|";
    for (int i = 0;i < process_count;i++){
        sum += norm_turn[i];
        ostringstream oss;
        oss << fixed << setprecision(2) << norm_turn[i];
        string norm = oss.str();
        int space = ceil(7 - (float)norm.size()) / 2;
        cout << right << setw(space) << norm << right << setw(space) << "|";
    }
    mean = sum / process_count;
    cout << right << setw(5) << fixed << setprecision(2) << mean << "|";


}

//tracing fn
void print_processes(){ 
    for (int i = 0;i < process_count;i++){
        cout << processes[i].name << ", " << processes[i].arrival << ", ";
        cout<< processes[i].service << endl;
    }
}

void FCFS(){
    sort_processes();
    int start_time = processes[0].arrival;

    for (int i = 0;i < process_count;i++){
        process p = processes[i];

        //calculating stats
        finish_time[i] = start_time + p.service;
        turn_around[i] = finish_time[i] - p.arrival;
        norm_turn[i] = (float)turn_around[i] / p.service;
        
        for (int j = start_time;j < finish_time[i];j++){
            timeline[i][j] = '*';
        }
        
        if (i+1 < process_count && (finish_time[i] > processes[i+1].arrival))
            start_time = finish_time[i];
        else
            start_time = processes[i+1].arrival;
    }
}

//returns index of shortest process (SPN helper)
int find_shortest_process(vector<process> pool){
    int min_index = 0;
    if (pool.size() == 1){
        return min_index;
    }
    for (int i = 1;i < (int)pool.size();i++){
        if ((pool[i].service < pool[min_index].service) && processes[i].done == false){
            min_index = i;
        }
    }

    return min_index;
}

//NOT DONE!!!!
void SPN(){
    // sort_processes();
    // int finished_processes = 0;
    // int start_time = processes[0].arrival;
    // vector<process> pool;

    // //checks which of the processes that arrive at the same time
    // for (int i = 0 ;i < process_count;i++){
    //     if ((processes[i].arrival == start_time) && processes[i].done == false){
    //         pool.push_back(processes[i]);
    //     }
    // }

    // int min_index = find_shortest_process(pool);
    // //-----------------------------------------------
    // cout << "min index: " << min_index << endl;
    // //-----------------------------------------------
    // while (finished_processes < process_count){
    //     process p = processes[min_index];

    //     //calculating stats
    //     finish_time[min_index] = start_time + p.service;
    //     turn_around[min_index] = finish_time[min_index] - p.arrival;
    //     norm_turn[min_index] = (float)turn_around[min_index] / p.service;

    //     for (int j = start_time;j < finish_time[min_index];j++){
    //         timeline[min_index][j] = '*';
    //     }
        
    //     processes[min_index].done = true; //p.done = true doesn't work (not a pointer) 
    //     finished_processes += 1;
    //     if (finished_processes == process_count){
    //         break;
    //     }
    //     start_time = finish_time[min_index];

    //     //-----------------------------------------------
    //     cout << "pool before:" << endl;
    //     for (int i = 0;i < (int)pool.size();i++){
    //         cout << pool[i].name << ", " << pool[i].arrival << endl;
    //     }
    //     cout << "------------------" << endl;
    //     //-----------------------------------------------

    //     //empties the pool
    //     pool.resize(0);
    //     //-----------------------------------------------
    //     cout << "pool:" << endl;
    //     for (int i = 0;i < (int)pool.size();i++){
    //         cout << pool[i].name << ", " << pool[i].arrival << endl;
    //     }
    //     cout << "------------------" << endl;
    //     //-----------------------------------------------
    //     //finds pool of candidate processes
    //     int  k = 0;
    //     while (processes[k].arrival <= start_time){
    //         cout << "process name inserted in pool:" << processes[k].name << ", " << processes[k].done << endl;
    //         pool.push_back(processes[k]);
    //         k++;
    //     }

    //     //if no process arrived before or when finishing the running process,
    //     //then the next process that must be chosen is the next process after the min index
    //     //since the processes are sorted according to arrival times 
    //     if (pool.empty()){
    //         pool.push_back(processes[min_index + 1]);
    //     }

    //     min_index = find_shortest_process(pool);
    //     cout << "min index: " << min_index << endl;
    // }    

    // reset_processes();
}

void RR(){

}

void SRT(){

}

void HRRN(){

}

void FB_1(){

}

void FB_2i(){

}

void Aging(){

}

void run(){
    for (int i = 0;i < (int)algorithms.size();i++){

        algorithm  alg = algorithms[i];

        if (operation == "trace"){
            switch (alg.id){
                case '1':
                    FCFS();
                    break;
                case '2':
                    RR();
                    break;
                case '3':
                    SPN();
                    break;
                case '4':
                    SRT();
                    break;
                case '5':
                    HRRN();
                    break;
                case '6':
                    FB_1();
                    break;
                case '7':
                    FB_2i();
                    break;
                case '8':
                    Aging();
                    break;
                default:
                    cout << "Invalid Algorithm" << endl;
            }

            print_timeline(i);

        }else if(operation == "stats"){
            switch (alg.id){
                case '1':
                    FCFS();
                    break;
                case '2':
                    RR();
                    break;
                case '3':
                    SPN();
                    break;
                case '4':
                    SRT();
                    break;
                case '5':
                    HRRN();
                    break;
                case '6':
                    FB_1();
                    break;
                case '7':
                    FB_2i();
                    break;
                case '8':
                    Aging();
                    break;
                default:
                    cout << "Invalid Algorithm" << endl;
            }
            
            print_stats(i);

        }else{
            cout << "Invalid Operation" << endl;
        }
    }
}

int main(){
    parse();
    print_processes();
    run();

    return 0;
}
