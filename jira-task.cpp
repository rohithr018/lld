#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <ctime>
#include <stdexcept>

using namespace std;

enum class Status {
    TODO,
    IN_PROGRESS,
    DONE,
    FAILED
};

string statusToString(Status status) {
    switch (status) {
        case Status::TODO:
            return "TODO";
        case Status::IN_PROGRESS:
            return "IN_PROGRESS";
        case Status::DONE:
            return "DONE";
        case Status::FAILED:
            return "FAILED";
    }
    return "";
}

struct Task {
    int id;
    string title;
    string assignee;

    Status status;

    string failureReason;

    long long createdAt;
    long long startedAt;

    Task() = default;

    Task(int id,
         const string& title,
         const string& assignee)
        : id(id),
          title(title),
          assignee(assignee),
          status(Status::TODO),
          failureReason(""),
          createdAt(time(nullptr)),
          startedAt(0) {}
};

class TaskTracker {

private:

    int nextTaskId = 1;

    unordered_map<int, Task> tasks;

    unordered_map<int, unordered_set<int>> statusIndex;

    unordered_map<string, unordered_set<int>> assigneeIndex;

    Task& getTaskInternal(int taskId) {

        auto it = tasks.find(taskId);

        if (it == tasks.end()) {
            throw invalid_argument(
                "Task " + to_string(taskId) +
                " does not exist"
            );
        }

        return it->second;
    }

    void changeStatus(int taskId,
                      Status newStatus) {

        Task& task = tasks[taskId];

        statusIndex[(int)task.status].erase(taskId);

        task.status = newStatus;

        statusIndex[(int)newStatus].insert(taskId);
    }

    [[noreturn]]
    void invalidTransition(const Task& task,
                           const string& action) {

        throw logic_error(
            "Task " + to_string(task.id) +
            " currently in status " +
            statusToString(task.status) +
            ", attempted " + action
        );
    }

public:

    int createTask(const string& title,
                   const string& assignee) {

        int id = nextTaskId++;

        Task task(id, title, assignee);

        tasks[id] = task;

        statusIndex[(int)Status::TODO].insert(id);

        assigneeIndex[assignee].insert(id);

        return id;
    }

    void start(int taskId) {

        Task& task = getTaskInternal(taskId);

        if (task.status != Status::TODO)
            invalidTransition(task, "start");

        task.startedAt = time(nullptr);

        changeStatus(taskId,
                     Status::IN_PROGRESS);
    }

    void complete(int taskId) {

        Task& task = getTaskInternal(taskId);

        if (task.status != Status::IN_PROGRESS)
            invalidTransition(task, "complete");

        changeStatus(taskId,
                     Status::DONE);
    }

    void fail(int taskId,
              const string& reason) {

        Task& task = getTaskInternal(taskId);

        if (task.status != Status::IN_PROGRESS)
            invalidTransition(task, "fail");

        task.failureReason = reason;

        changeStatus(taskId,
                     Status::FAILED);
    }

    void retry(int taskId) {

        Task& task = getTaskInternal(taskId);

        if (task.status != Status::FAILED)
            invalidTransition(task, "retry");

        task.failureReason.clear();

        task.startedAt = 0;

        changeStatus(taskId,
                     Status::TODO);
    }

    Task getTask(int taskId) {

        return getTaskInternal(taskId);
    }

    vector<Task> getByStatus(Status status) {

        vector<Task> result;

        for (int taskId : statusIndex[(int)status]) {
            result.push_back(tasks[taskId]);
        }

        return result;
    }

    vector<Task> getByAssignee(
            const string& assignee) {

        vector<Task> result;

        auto it = assigneeIndex.find(assignee);

        if (it == assigneeIndex.end())
            return result;

        for (int taskId : it->second) {
            result.push_back(tasks[taskId]);
        }

        return result;
    }
};

void printTask(const Task& task) {

    cout << "\nTask ID      : " << task.id
         << "\nTitle        : " << task.title
         << "\nAssignee     : " << task.assignee
         << "\nStatus       : "
         << statusToString(task.status)
         << "\nStarted At   : "
         << task.startedAt
         << "\nFailure      : "
         << task.failureReason
         << "\n";
}

int main() {

    TaskTracker tracker;

    cout << "===== CREATE TASK =====\n";

    int t1 =
        tracker.createTask(
            "Build login page",
            "alice");

    Task task1 =
        tracker.getTask(t1);

    printTask(task1);

    cout << "\n===== HAPPY PATH =====\n";

    tracker.start(t1);

    printTask(tracker.getTask(t1));

    tracker.complete(t1);

    printTask(tracker.getTask(t1));

    cout << "\n===== FAILURE PATH =====\n";

    int t2 =
        tracker.createTask(
            "Write tests",
            "bob");

    tracker.start(t2);

    tracker.fail(
        t2,
        "flaky environment");

    printTask(
        tracker.getTask(t2));

    cout << "\n===== RETRY =====\n";

    tracker.retry(t2);

    printTask(
        tracker.getTask(t2));

    cout << "\n===== GET BY STATUS =====\n";

    auto todoTasks =
        tracker.getByStatus(
            Status::TODO);

    for (auto& task : todoTasks)
        printTask(task);

    cout << "\n===== GET BY ASSIGNEE =====\n";

    auto aliceTasks =
        tracker.getByAssignee(
            "alice");

    for (auto& task : aliceTasks)
        printTask(task);

    cout << "\n===== INVALID TRANSITIONS =====\n";

    try {

        tracker.start(t1);

    } catch (const exception& e) {

        cout << e.what() << endl;
    }

    try {

        tracker.complete(t2);

    } catch (const exception& e) {

        cout << e.what() << endl;
    }

    try {

        tracker.getTask(999);

    } catch (const exception& e) {

        cout << e.what() << endl;
    }

    return 0;
}