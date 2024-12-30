#include <bits/stdc++.h> //includes almost all standard libraries
#include <format>
using namespace std; //uses std automatically
struct process{
   
    string name;
    int arrival;
    int service;
    bool done = false;
    int remaining_time; 
    int current_priority;
    int initial_priority;
    int waiting_time = 0;
    int index;
    // //remaining time must be initialized when parsing, 
    // //reamining time = service is wrong as service doesnt have a value yet
    void incrementPriority() {
        current_priority++;
    }
    void resetPriority() {
        current_priority = initial_priority;
    }
    bool operator==(const process& other) const {
        return name == other.name && arrival == other.arrival && service == other.service && done == other.done && remaining_time == other.remaining_time && current_priority == other.current_priority && initial_priority == other.initial_priority && waiting_time == other.waiting_time && index == other.index;
    }

    bool operator!=(const process& other) const {
        return name != other.name || arrival != other.arrival || service != other.service || done != other.done || remaining_time != other.remaining_time || current_priority != other.current_priority || initial_priority != other.initial_priority || waiting_time != other.waiting_time || index != other.index;
    }
   
};


struct ready_queue {
    std::vector<process> queue;

    // Add a process to the queue
    void enqueue(const process& p) {
        queue.push_back(p);
    }

    // Remove and return the front-most process
    process dequeue() {
        if (queue.empty()) {
            throw std::runtime_error("Queue is empty. Cannot dequeue.");
        }
        process front = queue.front();
        queue.erase(queue.begin());
        return front;
    }
     // Get the process with the highest priority
    process getHighestPriorityProcess() {
        if (queue.empty()) {
            throw std::runtime_error("Queue is empty. Cannot get highest priority process.");
        }
        return *std::max_element(queue.begin(), queue.end(),
            [](const process& a, const process& b) {
                return a.current_priority < b.current_priority; // Smaller priority value means higher priority
            });
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return queue.empty();
    }

     // Erase a specific process from the queue
    void erase(const process& p) {
        auto it = std::find(queue.begin(), queue.end(), p);
        if (it != queue.end()) {
            queue.erase(it);
        } else {
            throw std::runtime_error("Process not found in the queue.");
        }
    }
};
struct algorithm{
    char id; // 1->8
    int q = -1; //quantum for RR and Aging
};

//comparator for priority_queue
struct ComparePair {
    bool operator()(const pair<int, process>& a, const pair<int, process>& b) {
        return a.first > b.first; //compare only based on the first element
    }
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

//--------------------Parsing Section-------------------------------------------

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
    processes[i].remaining_time = service_time;

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

//------------------------------------------------------------------------------

//---------------------Helper Finctions-----------------------------------------

//resets the done and remaing_time variables of all processes
void reset_processes(){
    for (int i=0;i < process_count;i++){
        processes[i].done = false;
        processes[i].remaining_time = processes[i].service;
    }
}

//DONE (Sorts by arrival times)
void sort_processes(){
        sort(processes.begin(), processes.end(),
        [](const process& a, const process& b) {return a.arrival < b.arrival;});
}

//returns index of shortest process (SPN helper)
int find_shortest_process(const vector<process>& pool) {
    if (pool.empty()) 
        return -1;

    int min_index = 0;

    for (int i = 1; i < (int)pool.size(); i++) {
        //compare service times and choose the shortest
        if (pool[i].service < pool[min_index].service) {
            min_index = i;
        }
    }

    return min_index; 
}


void reset_timeline() {
    for(int i = 0;i < process_count;i++){
        for(int j = 0;j < last_instant;j++){
            timeline[i][j]= '.';
        }
    }
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

    for (int i = 0;i <= last_instant;i++){
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

    cout << endl;
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

//------------------------------------------------------------------------------

//DONE
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

//DONE
void SPN(){
    sort_processes(); // Ensure processes are sorted by arrival time
    int current_time = processes[0].arrival;

    while (true) {
        vector<process> ready_pool;

        // Build the pool of ready processes
        for (int i = 0; i < process_count; i++) {
            if (!processes[i].done && processes[i].arrival <= current_time) {
                ready_pool.push_back(processes[i]);
            }
        }

        // If the pool is empty and no process is left, break
        if (ready_pool.empty()) {
            bool all_done = true;
            for (int i = 0; i < process_count; i++) {
                if (!processes[i].done) {
                    all_done = false;
                    current_time = min(current_time, processes[i].arrival);
                    break;
                }
            }
            if (all_done) 
                break;
            else
                continue;
        }

        int shortest_index = find_shortest_process(ready_pool);
        if (shortest_index == -1) {
            //if no process is ready
            current_time++;
            continue;
        }

        //mapping back to the original processes vector
        int process_index = 0;
        for (int i = 0; i < process_count; i++) {
            if (processes[i].name == ready_pool[shortest_index].name) {
                process_index = i;
                break;
            }
        }

        process& p = processes[process_index];

        //calculating stats
        int start_time = current_time;
        finish_time[process_index] = start_time + p.service;
        turn_around[process_index] = finish_time[process_index] - p.arrival;
        norm_turn[process_index] = (float)turn_around[process_index] / p.service;

        for (int t = start_time; t < finish_time[process_index]; t++) {
            timeline[process_index][t] = '*';
        }

        //mark process as done and advance current time
        p.done = true;
        current_time = finish_time[process_index];
    }

}

//DONE (try other test cases)
void RR(int q){
    queue<int> p_queue;
    int k = 0;
    int j;
    while (k < process_count && processes[k].arrival == 0){
        p_queue.push(k);
        k++;
    }

    for (int i = 0;i < last_instant;i++){
        j = p_queue.front();
        p_queue.pop();
    
        int time_warp = i;
        while ((time_warp < i + q) && processes[j].remaining_time > 0){
            timeline[j][time_warp] = '*';
            processes[j].remaining_time--;
            time_warp++;
        }

        while (k < process_count && processes[k].arrival <= i+1){
            p_queue.push(k);
            k++;
        }

        if (processes[j].remaining_time == 0){
            finish_time[j] = time_warp;
            turn_around[j] = finish_time[j] - processes[j].arrival;
            norm_turn[j] = (float)turn_around[j] / processes[j].service;
        }else{
            p_queue.push(j);
        }

        i = time_warp-1;
    }

}

//DONE (try more tests)
void SRT(){
    //min_heap of remaining time and process index
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    int k = 0;
    pair<int, int> p;
    while (k < process_count && processes[k].arrival == 0){
        min_heap.push(make_pair(processes[k].remaining_time,k));
        k++;
    }

    for (int i = 0;i < last_instant;i++){
        p = min_heap.top();
        min_heap.pop();
        
        timeline[p.second][i] = '*';
        processes[p.second].remaining_time--;

        if (processes[p.second].remaining_time == 0){
            finish_time[p.second] = i;
            turn_around[p.second] = finish_time[p.second] - processes[p.second].arrival;
            norm_turn[p.second] = (float)turn_around[p.second] / processes[p.second].service;
        }else{
            min_heap.push(make_pair(processes[p.second].remaining_time, p.second));
        }

        while (k < process_count && processes[k].arrival <= i+1){
            min_heap.push(make_pair(processes[k].remaining_time, k));
            k++;
        }

    }

}


void HRRN(){

}

//DONE
void FB_1(){
    //min heap
    //pair<priority, process index>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    
    int k = 0;
    
    //find processes that arrive at time = 0
    while (k < process_count && processes[k].arrival == 0){
        min_heap.push(make_pair(0, k));
        k++;
    }
     
    int process_index;
    int priority;
    for (int i = 0;i < last_instant;i++){
        if (!min_heap.empty()){
            priority = min_heap.top().first;
            process_index = min_heap.top().second;
            min_heap.pop();

            timeline[process_index][i] = '*';
            processes[process_index].remaining_time--;

            //finds processes that will arrive at the next iteration
            //must be b4 pushing into min_heap
            while (k < process_count && processes[k].arrival == i+1){
                min_heap.push(make_pair(0, k));
                k++;       
            }

            if (processes[process_index].remaining_time == 0){
                //calculating stats
                finish_time[process_index] = i;
                turn_around[process_index] = finish_time[process_index] - processes[process_index].arrival;
                norm_turn[process_index] = (float)turn_around[process_index] / processes[process_index].service;

            }else{
                if (!min_heap.empty())
                    min_heap.push(make_pair(priority+1, process_index));
                else
                    min_heap.push(make_pair(priority, process_index));
            }

        }

        //finds processes that will arrive at the next iteration
        while (k < process_count && processes[k].arrival == i+1){
            min_heap.push(make_pair(0, k));
            k++;       
        }

    }

}

//DONE
void FB_2i(){
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    
    int k = 0;
    
    //find processes that arrive at time = 0
    while (k < process_count && processes[k].arrival == 0){
        min_heap.push(make_pair(0, k));
        k++;
    }
     
    int process_index;
    int priority;
    for (int i = 0;i < last_instant;i++){
        if (!min_heap.empty()){
            priority = min_heap.top().first;
            process_index = min_heap.top().second;
            min_heap.pop();

            int q = pow(2, priority);
            int time_warp = i;
            while ((time_warp < i + q) && processes[process_index].remaining_time > 0){
                timeline[process_index][time_warp] = '*';
                processes[process_index].remaining_time--;
                time_warp++;
            }


            //finds processes that will arrive at the next iteration
            while (k < process_count && processes[k].arrival <= i+1){
                min_heap.push(make_pair(0, k));
                k++;       
            }

            if (processes[process_index].remaining_time == 0){
                //calculating stats
                finish_time[process_index] = time_warp;
                turn_around[process_index] = finish_time[process_index] - processes[process_index].arrival;
                norm_turn[process_index] = (float)turn_around[process_index] / processes[process_index].service;

            }else{
                if (!min_heap.empty())
                    min_heap.push(make_pair(priority+1, process_index));
                else
                    min_heap.push(make_pair(priority, process_index));
            }

            i = time_warp - 1;
        }

        //finds processes that will arrive at the next iteration
        while (k < process_count && processes[k].arrival <= i+1){
            min_heap.push(make_pair(0, k));
            k++;       
        }

    }

}


void Aging(int q)
{
    ready_queue rq;
    // adjust priorities of all processes
    for (int i = 0; i < process_count; i++)
    {
        processes[i].initial_priority = processes[i].service;
        processes[i].current_priority = processes[i].service;
        processes[i].waiting_time = 0;
    }

    process max_priority_process;
    max_priority_process.name = "";
    int s = 0;
    for (int time = 0; time < last_instant; time++)
    {

        if (max_priority_process.name != "")
        {
            // remove the max_priority process from the ready queue
            rq.erase(max_priority_process); // correct erase
       
         while (s < process_count && processes[s].arrival <= time)
        {
            processes[s].index = s;
            rq.enqueue(processes[s]);
            s++;
        }
         for (size_t k = 0; k < rq.queue.size(); k++)
        { 
            rq.queue[k].incrementPriority();
            rq.queue[k].waiting_time++;
        }
        max_priority_process.resetPriority();
        max_priority_process.waiting_time = 0;
        rq.enqueue(max_priority_process);
            
            
        }
        else{
            while (s < process_count && processes[s].arrival <= time)
        {
            processes[s].index = s;
            rq.enqueue(processes[s]);
            s++;
        }

        }
        
        if (rq.isEmpty())
        {
            continue;
        }
     
        max_priority_process = rq.getHighestPriorityProcess();
        ready_queue copy_rq = rq; // correct copy
        // getting the max_prioritu process with low arrival time.
        while (!copy_rq.isEmpty())
        {
            process p = copy_rq.getHighestPriorityProcess();
            if (p.current_priority == max_priority_process.current_priority && p.waiting_time > max_priority_process.waiting_time)
            {
                max_priority_process = p;
            }
            copy_rq.erase(p);
        }

        // excuting the process
        int time_warp = time;
        while ((time_warp < time + q))
        {
            timeline[max_priority_process.index][time_warp] = '*';
            time_warp++;
        }
        time = time_warp - 1;

    }
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
                    RR(alg.q);
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
                    Aging(alg.q);
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
                    RR(alg.q);
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
                    Aging(alg.q);
                    break;
                default:
                    cout << "Invalid Algorithm" << endl;
            }
            
            print_stats(i);

        }else{
            cout << "Invalid Operation" << endl;
        }

        reset_timeline();
        reset_processes();
    }
}

int main(){
    parse();
    sort_processes();
    //print_processes();
    run();
    //FB_1();
    return 0;
}
