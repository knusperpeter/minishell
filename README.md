# 🐚 Minishell

A simplified bash-like shell implementation. This project is part of the 42 school curriculum and aims to provide a basic understanding of how shells work by implementing core shell functionalities.

## ⚠️ Notice to 42 Students
This repository serves as a reference. Please:
- Understand the code before using it
- Don't copy without comprehension
- Use it to learn concepts, not to bypass learning

## ✨ Features

### Core Functionality
- Interactive command prompt
- Command history navigation
- Path-based executable search
- Environment variable management
- Signal handling
- Exit status tracking (`$?`)

### Input/Output Operations
| Operation | Symbol | Description |
|-----------|--------|-------------|
| Input Redirection | `<` | Redirect input from file |
| Output Redirection | `>` | Redirect output to file |
| Here Document | `<<` | Read input until delimiter |
| Append Output | `>>` | Append output to file |
| Pipe | `\|` | Connect commands |

### Built-in Commands
| Command | Options | Description |
|---------|---------|-------------|
| `echo` | `-n` | Display text |
| `cd` | - | Change directory |
| `pwd` | - | Print working directory |
| `export` | - | Set environment variables |
| `unset` | - | Remove environment variables |
| `env` | - | Display environment |
| `exit` | - | Exit the shell |

### Signal Handling
| Signal | Key Combination | Behavior |
|--------|----------------|-----------|
| SIGINT | `Ctrl-C` | New prompt on new line |
| SIGQUIT | `Ctrl-\` | Ignored |
| EOF | `Ctrl-D` | Exit shell |

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Make
- Linux/Unix environment

### Installation
```bash
# Clone the repository
git clone https://github.com/caigner/minishell.git

# Navigate to project directory
cd minishell

# Compile
make
```

### Usage
```bash
# Run the shell
./minishell
```

## 💻 Examples

### Basic Commands
```bash
# Simple command
minishell$ ls -la

# Command with pipe
minishell$ ls | grep "file"

# Input/Output redirection
minishell$ cat < input.txt > output.txt
```

### Environment Variables
```bash
# Set variable
minishell$ export NAME=value

# Use variable
minishell$ echo $NAME

# Check exit status
minishell$ echo $?
```

## 🔧 Technical Details
- Written in C
- Follows 42 coding standards
- Memory leak-free
- Error handling for edge cases
- Process management for execution
- Custom parser implementation

## 👥 Contributors
- [Michael Heider](https://github.com/HeiMichael)

## 🤝 Acknowledgments
- 42 School for the project requirements
- Bash documentation for reference

---
*Built with dedication and understanding of shell mechanics* 🛠️
