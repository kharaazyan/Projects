# CPU Simulator

**Short Explanation:**  
This project is a simple CPU simulator implemented in C++. It supports a limited set of instructions (`ADD`, `SUB`, `MOV`, `LOAD`, `STORE`, `DISC`, `LAYO`, and `EXIT`) and simulates a basic register-based architecture with a memory array. The codebase is structured into multiple header and source files, making it more maintainable and extensible. A `Makefile` is provided to build the project.

**Good Example of Usage:**  
```bash
# Clone the repository (or copy the files into a directory)
git clone https://github.com/kharaazyan/CPUSimulator

cd CPUSimulator

# Build the project
make

# Run the simulator
./bin/cpu_sim
```

Once you run the program, you will see a prompt:
```
Type START to begin simulation or EXIT to quit:
> START
Initial CPU state:
R0:0 R1:0 ... IP:0 WSR:1
Memory: {0...0}

> ADD R1, R2, 5
> MOV R4, R1
> LOAD R3, 10
> STORE R2, 10
> LAYO
...
> EXIT
```

You can experiment by typing different instructions and observing how the register and memory states change.

---

**Detailed Explanation:**

### Overview
This CPU simulator is designed to mimic a very rudimentary CPU environment. It maintains a set of registers and a fixed-size RAM. Users can interactively type instructions to perform arithmetic operations, data movement, memory loads/stores, display the current layout (state), and exit the simulation.

### Features
- **Register Set:** R0 through R6, plus IP (Instruction Pointer) and WSR (a status register).
- **Memory Array:** A 256-byte array representing simple memory.
- **Instructions:**
  - **ADD Rdest, Rsrc, ValOrReg**: Adds the value of Rsrc and either a register or an immediate value, storing the result in Rdest.
  - **SUB Rdest, Rsrc, ValOrReg**: Subtracts from Rsrc either a register or an immediate value, storing the result in Rdest.
  - **MOV Rdest, RsrcOrVal**: Moves a register value or an immediate value into Rdest.
  - **LOAD Rdest, index**: Loads the value of Rdest into memory at the given index.
  - **STORE Rdest, index**: Stores the memory value at the given index into Rdest.
  - **DISC index**: Simulates a "rollback" of the CPU state from a record in the memory file (if applicable).
  - **LAYO**: Displays the current state of all registers and memory.
  - **EXIT**: Exits the simulation.

### Code Structure
The project is divided into multiple files and directories:

```
CPU/
├─ include/
│  ├─ constants.hpp     # Constants (RAM size, file paths, etc.)
│  ├─ instructions.hpp  # Enum for instruction types
│  ├─ registers.hpp     # Register structure definition
│  ├─ utils.hpp         # Utility function declarations (parsing, trimming, etc.)
│  └─ cpu.hpp           # The CPU class declaration
└─ src/
   ├─ main.cpp          # Entry point: initializes and runs the simulation
   ├─ cpu.cpp           # CPU class implementation
   ├─ utils.cpp         # Implementation of utility functions
   ├─ Makefile          # Makefile to build the project
   └─ (additional .cpp files if needed)
```

### Building the Project
- Ensure you have a C++17-compatible compiler (e.g., `g++`).
- Simply run:
  ```bash
  make
  ```
  This will create the `bin` directory (if it doesn’t exist) and produce the `cpu_sim` executable inside it.

### Running the Simulation
After building, run:
```bash
./bin/cpu_sim
```

You will be prompted to type `START` or `EXIT`. If you type `START`, the simulation begins, and you can input CPU instructions. If you type `EXIT`, the simulator quits.

### Example Session
```
Type START to begin simulation or EXIT to quit:
> START
Initial CPU state:
R0: 0, R1: 0, R2: 0, R3: 0, R4: 0, R5: 0, R6: 0, WSR: 1, IP: 0
Memory: {0...0}

> MOV R1, 10      # R1 = 10
> MOV R2, R1       # R2 = 10
> ADD R3, R1, R2   # R3 = 10 + 10 = 20
> LAYO
CPU state:
R0: 0, R1: 10, R2: 10, R3: 20, R4: 0, R5: 0, R6: 0, WSR: 1, IP: 3
Memory: { ... }

> EXIT
You have successfully exited the simulation
```

### Extensibility
- To add new instructions, modify `instructions.hpp` and add corresponding parsing and execution logic in `cpu.cpp`.
- The `utils.cpp` file contains general-purpose functions for command parsing, making it easier to extend the syntax.

### Troubleshooting
- If you encounter warnings or unused variables, you can simply remove or comment out the offending code lines.
- Ensure that `/tmp/memory.txt` is writable by your user. The simulator uses this file for saving/restoring CPU states.