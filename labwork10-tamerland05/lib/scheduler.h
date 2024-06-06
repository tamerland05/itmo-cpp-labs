#include <vector>
#include <memory>

class TTaskScheduler {
private:
    struct MyVoid {};

    template<class Type>
    struct Checker {
        typedef Type type;
    };

    template<class Func, class Arg1, class Arg2>
    struct CheckFunc {
        auto operator()(const Arg1& arg1, const Arg2& arg2) {
            return Checker<decltype(Func{}(arg1, arg2))>{};
        }
    };

    template<class Func, class Arg1>
    struct CheckFunc<Func, Arg1, MyVoid> {
        auto operator()(const Arg1& arg1, const MyVoid&) {
            return Checker<decltype(Func{}(arg1))>{};
        }
    };

    template<class Func>
    struct CheckFunc<Func, MyVoid, MyVoid> {
        auto operator()(const MyVoid&, const MyVoid&) {
            return Checker<decltype(Func{}())>{};
        }
    };

    template<class Func, class RetType, class Arg1, class Arg2>
    struct SmartExecute {
        void operator()(const Arg1& arg1, const Arg2& arg2, std::shared_ptr<RetType>& result) {
            *result = Func{}(arg1, arg2);
        }
    };

    template<class Func, class RetType, class Arg1>
    struct SmartExecute<Func, RetType, Arg1, MyVoid> {
        void operator()(const Arg1& arg1, const MyVoid&, std::shared_ptr<RetType>& result) {
            *result = Func{}(arg1);
        }
    };

    template<class Func, class RetType>
    struct SmartExecute<Func, RetType, MyVoid, MyVoid> {
        void operator()(const MyVoid&, const MyVoid&, std::shared_ptr<RetType>& result) {
            *result = Func{}();
        }
    };

    class BaseTask {
        friend class TTaskScheduler;

        virtual void Execute(std::vector<std::shared_ptr<BaseTask>>&) = 0;
        bool is_executed = false;
        std::vector<size_t> previous_tasks_;
    };

    template<class Type>
    struct FutureObject {
        const size_t task_num_{};
        std::shared_ptr<Type> result_{new Type};
    };

    template<class Func, class Arg1, class Arg2, class RetType>
    class Task : public BaseTask {
        friend class TTaskScheduler;

        const Arg1& arg1_;
        const Arg2& arg2_;
        FutureObject<RetType> future_object;

        Task(const Func&, const Arg1& arg1, const Arg2& arg2, size_t task_num)
                : arg1_(arg1), arg2_(arg2), future_object(task_num) {}

        void Execute(std::vector<std::shared_ptr<BaseTask>>& tasks) override {
            if (is_executed) return;

            for (auto& own_task : previous_tasks_) {
                tasks[own_task]->Execute(tasks);
            }

            is_executed = true;
            SmartExecute<Func, RetType, Arg1, Arg2>{}(arg1_, arg2_, future_object.result_);
        }
    };

    std::vector<std::shared_ptr<BaseTask>> all_tasks_;

public:
    template<class Func, class Arg1=MyVoid, class Arg2=MyVoid>
    auto& Add(const Func& func, const Arg1& arg1=MyVoid{}, const Arg2& arg2=MyVoid{}) {
        using ret_type = typename decltype(CheckFunc<Func, Arg1, Arg2>{}(arg1, arg2))::type;

        auto task_ptr = new Task<Func, Arg1, Arg2, ret_type>(func, arg1, arg2, all_tasks_.size());
        all_tasks_.emplace_back(static_cast<BaseTask *>(task_ptr));

        return *task_ptr;
    }

    template<class Func, class Arg1, class Type>
    auto& Add(const Func& func, const Arg1& arg1, FutureObject<Type>& arg2) {
        auto& task = Add(func, arg1, *arg2.result_);
        task.previous_tasks_.emplace_back(arg2.task_num_);

        return task;
    }

    template<class Func, class Type, class Arg2=MyVoid>
    auto& Add(const Func& func, FutureObject<Type>& arg1, const Arg2& arg2=MyVoid{}) {
        auto& task = Add(func, *arg1.result_, arg2);
        task.previous_tasks_.emplace_back(arg1.task_num_);

        return task;
    }

    template<class Func, class Type1, class Type2>
    auto& Add(const Func& func, FutureObject<Type1>& arg1, FutureObject<Type2>& arg2) {
        auto& task = Add(func, *arg1.result_, *arg2.result_);
        task.previous_tasks_.emplace_back(arg1.task_num_);
        task.previous_tasks_.emplace_back(arg2.task_num_);

        return task;
    }

    template<class Type, class TaskType>
    FutureObject<Type>& GetFutureResult(TaskType& my_task) {
        return my_task.future_object;
    }

    template<class Type, class TaskType>
    Type& GetResult(TaskType& my_task) {
        my_task.Execute(all_tasks_);

        return *my_task.future_object.result_;
    }

    void ExecuteAll() {
        for (auto& my_task : all_tasks_) {
            my_task->Execute(all_tasks_);
        }
    }
};
