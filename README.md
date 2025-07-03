# Philosophers

## 📖 Project Overview

- The Philosophers project is part of the 42 curriculum designed to introduce students to multithreading and process synchronization in C. It is a simulation of the classic "Dining Philosophers Problem," where philosophers alternate between eating, sleeping, and thinking while sharing limited resources (forks).

- The challenge lies in correctly managing concurrent access to forks and ensuring that no philosopher starves or causes a deadlock.

---

## 📝 Mandatory Objectives

- ### 1. Program Name

- ./philo

- ### 2. Arguments:

- `number_of_philosophers` `time_to_die time_to_eat` `time_to_sleep` `[number_of_times_each_philosopher_must_eat]`

- `number_of_philosophers`: The number of philosophers and forks (must be > 0).

- `time_to_die`: Time (in milliseconds) a philosopher can go without eating before dying.

- `time_to_eat`: Time (in milliseconds) it takes for a philosopher to eat.

- `time_to_sleep`: Time (in milliseconds) a philosopher spends sleeping.

- `[number_of_times_each_philosopher_must_eat]`: Optional. If specified, simulation stops when all philosophers have eaten this many times.

---

## ⚙️ Program Rules

- Each philosopher `must think` → `take forks` → `eat` → `release forks` → `sleep` in a continuous loop.

- Philosophers must not die `(starve)` if resources are available.

- The simulation ends if:

    - A philosopher dies.

    - All philosophers have eaten the required number of times (if specified).

- Output must be timestamped and include philosopher ID and current action:
```
800 1 is eating
2000 3 died
```

----
## 🔑 Key Concepts to Implement

- ### Threads:

    - Each philosopher runs in its own thread.

- ### Mutexes:

    - Using `mutexes` to protect shared resources (forks).

- ### Timers:

    - Using `gettimeofday` or equivalent to manage precise timing.

- ### Synchronization:

    - Ensure philosophers never:

        - Eat without both forks.

        - Cause deadlocks by improper fork grabbing order.

- ### Death Detection:

    - A monitor thread or function must check regularly if any philosopher exceeds time_to_die.

----

## 🔗 Sample Command

- ./philo 5 800 200 200

    - 5 philosophers

    - Die after 800ms without eating

    - Eat for 200ms

    - Sleep for 200ms

----

# 📚 Useful Functions:

- `pthread_create`

- `pthread_join`

- `pthread_mutex_init`

- `pthread_mutex_lock`

- `pthread_mutex_unlock`

- `gettimeofday`

- `usleep`

----

# 🚀 Conclusion

The Philosophers project teaches essential concurrency skills by simulating a classic problem that is still relevant in operating systems, parallel programming, and resource management.

Focus on clean, synchronized code and precise time management to meet the mandatory requirements successfully.
