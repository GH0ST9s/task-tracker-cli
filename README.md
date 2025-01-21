# Task-Tracker CLI Tool

A simple and efficient command-line To-Do List tracking tool written in C++. This program allows users to add, view, and manage tasks directly from the terminal.

## Features

- Add new tasks to the to-do list.
- Mark tasks as to-do, in-progress, done.
- View all or specific tasks with their statuses.
- Delete tasks when no longer needed.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/todo-list-cli.git

2. Navigate to the project directory:
   cd task-tracker-cli

3. Compile the program (requires a C++ compiler like g++):
   g++ task-tracker.cpp -o task-tracker

## Usage
  # Adding a new task
  task-tracker add "Buy groceries"
  Output: Task added successfully (ID: 1)

  # Updating and deleting tasks
  task-tracker update 1 "Buy groceries and cook dinner"
  task-tracker delete 1

  # Marking a task as in progress or done
  task-tracker mark-in-progress 1
  task-tracker mark-done 1

  # Listing all tasks
  task-tracker list

  # Listing tasks by status
  task-tracker list done
  task-tracker list todo
  task-tracker list in-progress

## Requirements
  C++17 or later.

## Contributing
  Contributions are welcome! Feel free to submit a pull request or open an issue for suggestions or bugs.

## Roadmap.sh
  https://roadmap.sh/projects/task-tracker
