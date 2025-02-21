# CPU-Scheduling-Algorithms-Sim

## Description
CPU-Scheduling-Algorithms-Sim is a C++ project that simulates various CPU scheduling algorithms. The purpose of this project is to demonstrate how different scheduling algorithms manage process execution in an operating system.

## Features
- Simulates multiple CPU scheduling algorithms such as:
  - First-Come, First-Served (FCFS)
  - Shortest Job Next (SJN)
  - Priority Scheduling
  - Round Robin (RR)
  - Multilevel Queue Scheduling
- Provides detailed output for each scheduling algorithm to visualize process execution order and timing.
- Easy-to-understand code structure for educational purposes.

## Installation
To run this project locally, follow these steps:

1. **Clone the repository:**
   ```sh
   git clone https://github.com/AlyKhalil/CPU-Scheduling-Algorithms-Sim.git
   cd CPU-Scheduling-Algorithms-Sim
   ```

2. **Compile the code:**
   ```sh
   g++ -o scheduler main.cpp
   ```

3. **Run the executable:**
   ```sh
   ./scheduler
   ```

## Usage
To use the simulation, follow the instructions provided in the console. You will be prompted to enter process details and select the scheduling algorithm to simulate.

Example usage:
```
Enter the number of processes: 3
Enter process details (ID, arrival time, burst time, priority):
1 0 5 2
2 2 3 1
3 4 1 3

Select scheduling algorithm:
1. FCFS
2. SJN
3. Priority Scheduling
4. Round Robin
5. Multilevel Queue Scheduling

Enter your choice: 1
```

## Requirements
- C++ compiler (e.g., g++)

## Contributing
Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -m 'Add some feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Create a pull request.

## License
This project is licensed under the MIT License.

## Acknowledgments
- This project was created for educational purposes to demonstrate CPU scheduling algorithms.
- Special thanks to the contributors and maintainers of the C++ language and compiler tools.

## Contact
For any questions or suggestions, please contact [AlyKhalil](https://github.com/AlyKhalil).
