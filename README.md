# Codexion
*This project has been created as part of the 42 curriculum by zhewu*

## Description

**Codexion** is a multi-threaded simulation of quantum coders working in a collaborative co-working hub. Coders (threads) need to acquire two USB dongles simultaneously to compile their quantum code. Once compilation is complete, they debug, then refactor, and repeat the cycle. The simulation demonstrates concurrency control, resource sharing, deadlock prevention, and scheduling algorithms in a real-time environment.

The simulation stops when either:
- A coder burns out (fails to start compiling within their deadline), or
- All coders have successfully compiled a required number of times.

## Instructions

### Installation:
1. Clone the repository and navigate into the project directory:
```bash
git clone <Git repository> Codexion
cd Codexion
```

### Compilation

```bash
make
```

This compiles the `codexion` executable using `cc` with flags `-Wall -Wextra -Werror -pthread`.

### Cleaning

```bash
make clean     # Remove object files
make fclean    # Remove object files and executable
make re        # Clean and recompile
```

### Usage

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Arguments:

| Argument | Description | Unit |
|----------|-------------|------|
| `number_of_coders` | Number of coder threads and dongles | count |
| `time_to_burnout` | Max time without compiling before burnout | milliseconds |
| `time_to_compile` | Time spent holding both dongles | milliseconds |
| `time_to_debug` | Time spent debugging after compile | milliseconds |
| `time_to_refactor` | Time spent refactoring after debug | milliseconds |
| `number_of_compiles_required` | Required compiles per coder to succeed | count |
| `dongle_cooldown` | Time dongle is unavailable after release | milliseconds |
| `scheduler` | Arbitration policy: `fifo` or `edf` | - |

#### Examples:

```bash
# Basic FIFO simulation with 2 coders
./codexion 2 10000 500 200 300 3 100 fifo

# EDF simulation with 3 coders
./codexion 3 5000 300 50 50 5 50 edf

# Burnout scenario (short burnout time)
./codexion 2 150 300 100 100 2 20 fifo
```

#### Output Format:

```
<timestamp_ms> <coder_id> has taken a dongle
<timestamp_ms> <coder_id> is compiling
<timestamp_ms> <coder_id> is debugging
<timestamp_ms> <coder_id> is refactoring
<timestamp_ms> <coder_id> burned out
```

Example:
```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
300 1 is debugging
350 1 is refactoring
```

## Blocking Cases Handled

### Deadlock Prevention
The implementation prevents **Coffman's conditions** to prevent deadlock:

1. **Mutual Exclusion**: Each dongle is protected by a mutex; only one coder can hold it at a time.
3. **No Preemption**: Dongles are released voluntarily after compilation completes.
4. **Circular Wait**: Eliminated by each coder trying to grab 2 dongles at the same time.

### Starvation Prevention
- **FIFO scheduler**: Requests are queued in arrival order, ensuring eventual service.
- **EDF scheduler**: Coders with earliest deadlines (last_compile_start + time_to_burnout) are prioritized, preventing any coder from being indefinitely postponed.

### Dongle Cooldown
- After a dongle is released, it enters a cooldown period where it cannot be acquired.

### Precise Burnout Detection
- A dedicated monitor thread periodically checks each coder's deadline.
- Burnout is detected within 10ms of the actual missed deadline.

### Log Serialization
- A mutex protects all log output operations.
- Messages are never interleaved; each line contains a complete, atomic log entry.

## Thread Synchronization Mechanisms

### pthread_mutex_t
- **Dongle mutexes**: One mutex per dongle protects dongle state (available/cooldown, holder, last_release_time).
- **Log mutex**: Serializes all output to prevent interleaved messages.

### pthread_cond_t
- **Dongle prioritization condition variables**: Each dongle has a condvar for waiting coders. When a dongle becomes available or cooldown ends, `pthread_cont_t cv;` is broadcasted.

### Priority Queue (Custom Heap Implementation)
- No standard library priority queue is used.
- For EDF, heap ordering is based on coder deadlines (last_compile_start + time_to_burnout), and threads are controlled by `pthread_cont_t cv;`
- For FIFO, heap ordering is based on request arrival time.

### Thread Communication
- Monitor thread uses `termination_signal` to inform coder threads the end of simulation.
- After activating `termination_signal`, no logs are printed to console and the program stops soon.

## Resources

### Documentation
- [Multithreaded Programming Guide](https://docs.oracle.com/cd/E37838_01/html/E61057/index.html) - Guide of thread functions and variables required for this project
- [Thread Management Functions in C](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/) - Explanation of Thread Management functions in C language
- [Coffman's conditions for deadlock](https://en.wikipedia.org/wiki/Deadlock_(computer_science))

### AI Usage
Ai was used in the following ways for this project:

- **Documentation assistance:** AI helped with structuring and formatting this README.md file
- **Conceptual Understanding:** AI was used to research and better understand different sorting algorithms and their complexities.

**Note:** All configurations were manually implemented and tested. AI was used as a learning aid to understand concepts, not to generate solutions.
