*This project has been created as part of the 42 curriculum by madelwau*

Philosophers
============

Description
-----------
**Philosophers** is a project designed around the classic Dining Philosophers problem, a famous computer science puzzle illustrating synchronization issues in concurrent programming. 

The core objective is to simulate a circle of philosophers sitting at a round table, alternating between eating, sleeping, and thinking. There is a single bowl of spaghetti in the middle of the table, and exactly as many forks as philosophers. Because eating requires two forks, philosophers must share their adjacent resources, leading to potential concurrency conflicts.

This implementation focuses exclusively on the **Mandatory Part** of the assignment:
* Each philosopher is represented as an independent **POSIX thread (`pthread`)**.
* Shared forks are guarded using **mutexes (`pthread_mutex_t`)** to prevent concurrent access and data duplication.
* The program handles time synchronization down to the millisecond to ensure real-time precision and accurate death tracking.

---

Technical Choices & Optimization
--------------------------------
To achieve optimal performance and strict adherence to the 42 subject rules (such as zero data races and avoiding deadlocks), the following architecture was implemented:

* **Asymmetric Resource Acquisition:** Philosophers are ordered by ID. Even-numbered philosophers pick up their right fork first and then their left, while odd-numbered philosophers pick up their left fork first. This breaks the circular dependency and completely eliminates the risk of **Deadlocks**.
* **Precise Timing Mechanism:** Standard `usleep()` can lag due to OS scheduling. A custom high-precision sleep function (`ft_usleep`) was coded. It polls `gettimeofday()` in micro-intervals, allowing for an active-wait loop that breaks immediately if a death condition is met, preventing delayed death logs.
* **Thread-Safe Shared States:** State variables accessible by multiple threads (such as the death flag or individual meal timestamps) are systematically isolated and guarded by specific mutexes (`dead_mutex`, `meal_mutex`, `write_mutex`) to avoid **Data Races**.

---

Instructions
------------
### Compilation
The program compiles into an executable named `philo` inside the `philo/` directory. The provided `Makefile` compiles all source files using `cc` with the flags `-Wall -Wextra -Werror` and prevents unnecessary relinking.

To compile the project, navigate to the `philo/` directory and run:
```bash
make
```
Available rules:
- make / make all: Compiles the philo executable.
- make clean: Removes all compiled object files (.o).
- make fclean: Removes object files and the philo binary.
- make re: Cleans and recompiles the entire project.

Execution
The program accepts 4 mandatory arguments and 1 optional argument:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
- number_of_philosophers: The number of philosophers sitting at the table (and the number of available forks).
- time_to_die (in ms): If a philosopher does not start eating within this timeframe since their last meal or the start of the simulation, they die.
- time_to_eat (in ms): The duration a philosopher takes to eat while holding both forks.
- time_to_sleep (in ms): The duration a philosopher spends sleeping.
- [number_of_times_each_philosopher_must_eat] (optional): If specified, the simulation stops cleanly once all philosophers have eaten at least this many times. If omitted, the simulation runs until a philosopher dies.

Usage Examples
```bash
# Case where 1 philosopher should die of starvation at 800ms
./philo 1 800 200 200

# Normal infinite simulation where everyone should survive (Ctrl+C to exit)
./philo 4 410 200 200

# Simulation that terminates automatically after everyone eats at least 7 times
./philo 5 800 200 200 7

# Heavy stress-test with 200 threads (should not crash or waste CPU capacity)
./philo 200 600 600 600
```

Resources
---------
Documentation & References
- POSIX Threads Programming: [LLNL POSIX Threads Introduction](https://computing.llnl.gov/tutorials/pThreads/) — A great reference manual for thread lifecycle management, mutexes, and join/detach behaviors.
- The Dining Philosophers Problem: [Wikipedia - Dining Philosophers](https://en.wikipedia.org/wiki/Dining_philosophers_problem) — Contextual background on concurrent algorithm designs and resource contention.
- Thread Sanitizer: [Google Sanitalizers Wiki](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual) — Documentation on how to use the -fsanitize=thread compilation flag to monitor memory access patterns during execution.

AI Usage Disclosure
-------------------
In compliance with the 42 AI Instructions guidelines, generative AI was utilized during this project as follows:
- Task Scope: AI assisted in drafting initial boilerplate code architectures for structural separation, optimizing the mathematical polling logic for ft_usleep, and formatting this documentation file.
- Verification Process: Every block of logic proposed by the AI was cross-referenced with the official pthread man pages, thoroughly stress-tested against the Thread Sanitizer to ensure thread safety, and manually adjusted to fit the structural and stylistic demands of the 42 Norm.
