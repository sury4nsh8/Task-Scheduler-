#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

class Task {
public:
    string name;
    string description;
    string due_date;
    int priority;
    set<string> dependencies;

    
    bool completed;
    string remarks;

    Task(const string& n, const string& desc, const string& date, int prio)
        : name(n), description(desc), due_date(date), priority(prio), completed(false) {}
};

class Employee {
public:
    string name;
    string department;
    vector<Task> tasks;

    Employee(const string& n, const string& dept) : name(n), department(dept) {}
};

class TaskList {
private:
    map<string, vector<Task>> employeeTasks;
    vector<Task> tasks;
    priority_queue<Task, vector<Task>, function<bool(const Task&, const Task&)>> taskQueue;

public:
    TaskList() : taskQueue([](const Task& t1, const Task& t2) { return t1.priority < t2.priority; }) {}

    void addTask(const Task& task) {
        tasks.push_back(task);
        taskQueue.push(task);
    }

    void assignTaskToEmployee(const string& employeeID, const Task& task) {
        employeeTasks[employeeID].push_back(task);
    }

    void removeTask(const string& taskName) {
        tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](const Task& task) {
            return task.name == taskName;
        }), tasks.end());
    }

    void updateTask(const string& taskName, const Task& newTask) {
        for (auto& task : tasks) {
            if (task.name == taskName) {
                task = newTask;
                break;
            }
        }
    }

    void displayTasks() {
        for (const Task& task : tasks) {
            cout << "Task: " << task.name << endl;
            cout << "Description: " << task.description << endl;
            cout << "Due Date: " << task.due_date << endl;
            cout << "Priority: " << task.priority << endl;
            cout << "---------------------" << endl;
        }
    }

    void displayHighestPriorityTasks() {
        if (tasks.empty()) {
            cout << "No tasks assigned yet." << endl;
            return;
        }

        cout << "Tasks with the highest priority:" << endl;
        while (!taskQueue.empty()) {
            Task task = taskQueue.top();
            cout << "Task: " << task.name << endl;
            cout << "Description: " << task.description << endl;
            cout << "Due Date: " << task.due_date << endl;
            cout << "Priority: " << task.priority << endl;
            cout << "---------------------" << endl;
            taskQueue.pop();
        }
    }

    vector<Task> getTasksByDueDate() {
        vector<Task> tasksByDueDate = tasks;
        sort(tasksByDueDate.begin(), tasksByDueDate.end(), [](const Task& t1, const Task& t2) {
            return t1.due_date < t2.due_date;
        });
        return tasksByDueDate;
    }

    vector<Task> getTasksForEmployee(const string& employeeID) {
        if (employeeTasks.find(employeeID) != employeeTasks.end()) {
            // Get the tasks for the employee
            vector<Task>& employeeTasksList = employeeTasks[employeeID];

            // Sort the tasks by priority (high to low)
            sort(employeeTasksList.begin(), employeeTasksList.end(), [](const Task& t1, const Task& t2) {
                return t1.priority > t2.priority;
            });

            return employeeTasksList;
        }
        return {};
    }
};

// Function to mark a task as completed and add remarks
void markTaskAsCompleted(Task& task) {
    task.completed = true;
    cout << "Task marked as completed." << endl;

    // Allow admin to add remarks
    cout << "Enter remarks: ";
    cin.ignore();
    getline(cin, task.remarks);
}

// Function to manage tasks for employees
void employeeTaskManagement(const string& employeeID, TaskList& taskList) {
    vector<Task> tasks = taskList.getTasksForEmployee(employeeID);

    if (tasks.empty()) {
        cout << "No tasks assigned to you yet." << endl;
        return;
    }

    cout << "Your tasks:" << endl;
    for (Task task : tasks) {
        cout << "Task: " << task.name << endl;
        cout << "Description: " << task.description << endl;
        cout << "Due Date: " << task.due_date << endl;
        cout << "Priority: " << task.priority << endl;

        if (!task.completed) {
            cout << "1. Mark as done" << endl;
        }

        if (!task.remarks.empty()) {
            cout << "Remarks: " << task.remarks << endl;
        }

        if (!task.completed) {
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1) {
                markTaskAsCompleted(task);
            }
        }

        cout << "---------------------" << endl;
    }
}

int main() {
    TaskList taskList;

    Task sampleTask1("Task 1", "Description for Task 1", "2023-11-01", 3);
    Task sampleTask2("Task 2", "Description for Task 2", "2023-11-05", 2);
    Task sampleTask3("Task 3", "Description for Task 3", "2023-11-03", 1);

    taskList.addTask(sampleTask1);
    taskList.addTask(sampleTask2);
    taskList.addTask(sampleTask3);

    string employeeNames[] = {"Employee 1", "Employee 2", "Employee 3"};

    while (true) {
        cout << "-------Welcome to Task Manager and Optimizer! By CodaHolics-------" << endl;
        cout << "Are you an Admin or an Employee?" << endl;
        cout << "Press 1 for Admin" << endl;
        cout << "Press 2 for Employee" << endl;
        cout << "Press 3 to Exit" << endl;

        int userRole;
        cin >> userRole;

        if (userRole == 3) {
            break;
        } else if (userRole == 1) {
            cout << "Select a department:" << endl;
            cout << "Press 1 for HR team" << endl;
            cout << "Press 2 for Technical Team" << endl;
            cout << "Press 3 for Marketing Team" << endl;

            int deptChoice;
            cout << "Enter your choice: " << endl;
            cin >> deptChoice;
            cin.ignore();

            if (deptChoice >= 1 && deptChoice <= sizeof(employeeNames) / sizeof(employeeNames[0])) {
                const string adminID = employeeNames[deptChoice - 1];
                cout << "List of Employees under this department:" << endl;

                for (const string& employeeName : employeeNames) {
                    cout << "- " << employeeName << endl;
                }

                while (true) {
                    cout << "Select an employee to manage tasks (Enter 0 to return to the main menu): ";
                    int employeeChoice;
                    cin >> employeeChoice;
                    cin.ignore();

                    if (employeeChoice == 0) {
                        break;
                    } else if (employeeChoice > 0 && employeeChoice <= sizeof(employeeNames) / sizeof(employeeNames[0])) {
                        const string employeeID = employeeNames[employeeChoice - 1];

                        while (true) {
                            cout << "1. Add Task" << endl;
                            cout << "2. Remove Task" << endl;
                            cout << "3. Update Task" << endl;
                            cout << "4. Display Tasks" << endl;
                            cout << "5. Review Tasks" << endl;
                            cout << "6. Choose another employee under the same team" << endl;
                            cout << "Enter your choice: ";
                            int choice;
                            cin >> choice;

                            if (choice == 1) {
                                string name, description, due_date;
                                int priority;
                                cout << "Enter task name: ";
                                cin.ignore();
                                getline(cin, name);
                                cout << "Enter task description: ";
                                getline(cin, description);
                                cout << "Enter due date: ";
                                cin >> due_date;
                                while (true) {
                                    cout << "Enter new priority (1-5): ";
                                    cin >> priority;
                                    if (priority > 5 || priority < 1) {
                                        cout << "Enter a valid priority level!!!" << endl;
                                        continue;
                                    } else {
                                        break;
                                    }
                                }
                                Task newTask(name, description, due_date, priority);

                                taskList.assignTaskToEmployee(employeeID, newTask);

                                cout << "Task assigned to Employee with ID: " << employeeID << endl;
                            } else if (choice == 2) {
                                cout << "Enter the task name to remove: ";
                                string taskName;
                                cin.ignore();
                                getline(cin, taskName);

                                // Find the task to remove by name
                                vector<Task> tasks = taskList.getTasksForEmployee(employeeID);
                                for (auto it = tasks.begin(); it != tasks.end(); ++it) {
                                    if (it->name == taskName) {
                                        tasks.erase(it);
                                        cout << "Task removed from Employee with ID: " << employeeID << endl;
                                        break;
                                    }
                                }
                            } else if (choice == 3) {
                                cout << "Enter the task name to update: ";
                                string taskName;
                                cin.ignore();
                                getline(cin, taskName);

                                // Find the task to update by name
                                vector<Task> tasks = taskList.getTasksForEmployee(employeeID);
                                for (Task& task : tasks) {
                                    if (task.name == taskName) {
                                        string name, description, due_date;
                                        int priority;
                                        cout << "Enter new task name: ";
                                        cin.ignore();
                                        getline(cin, name);
                                        cout << "Enter new task description: ";
                                        getline(cin, description);
                                        cout << "Enter new due date: ";
                                        cin >> due_date;
                                        while (true) {
                                            cout << "Enter new priority (1-5): ";
                                            cin >> priority;
                                            if (priority > 5 || priority < 1) {
                                                cout << "Enter a valid priority level!!!" << endl;
                                                continue;
                                            } else {
                                                break;
                                            }
                                        }
                                        Task updatedTask(name, description, due_date, priority);
                                        task = updatedTask;
                                        cout << "Task updated for Employee with ID: " << employeeID << endl;
                                        break;
                                    }
                                }
                            } else if (choice == 4) {
                                // Display tasks for the selected employee
                                cout << "Tasks for Employee with ID: " << employeeID << endl;
                                const vector<Task> tasks = taskList.getTasksForEmployee(employeeID);
                                if (tasks.empty()) {
                                    cout << "No tasks assigned to this employee yet." << endl;
                                } else {
                                    for (const Task& task : tasks) {
                                        cout << "Task: " << task.name << endl;
                                        cout << "Description: " << task.description << endl;
                                        cout << "Due Date: " << task.due_date << endl;
                                        cout << "Priority: " << task.priority << endl;
                                        cout << "Status: " << (task.completed ? "Completed" : "Not Completed") << endl;
                                        cout << "Remarks: " << task.remarks << endl;
                                        cout << "---------------------" << endl;
                                    }
                                }
                            } else if (choice == 5) {
                                taskList.displayTasks();
                                cout << "Enter task name to add remarks: ";
                                string taskName;
                                cin.ignore();
                                getline(cin, taskName);
                                for (Task& task : taskList.getTasksForEmployee(employeeID)) {
                                    if (task.name == taskName) {
                                        cout << "Enter remarks: ";
                                        cin.ignore();
                                        getline(cin, task.remarks);
                                        break;
                                    }
                                }
                            } else if (choice == 6) {
                                break;
                            }
                        }
                    } else {
                        cout << "Invalid choice. Please select a valid employee or enter 0 to return to the main menu." << endl;
                    }
                }
            } else {
                cout << "Invalid department choice. Please select a valid department." << endl;
            }
        } else if (userRole == 2) {
            cout << "Enter your employee ID: ";
            string employeeID;
            cin.ignore();
            getline(cin, employeeID);
            employeeTaskManagement(employeeID, taskList);
        } else {
            cout << "Invalid choice. Please select 1 for Admin or 2 for Employee." << endl;
        }
    }

    return 0;
}