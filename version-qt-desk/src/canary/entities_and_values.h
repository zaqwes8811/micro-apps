#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <set>
#include <string>
#include <vector>

namespace values {
class ImmutableTask;
}

namespace entities {
const std::string gTableName = "tasks";

struct EntitiesStates {
  static const int kInActiveKey = -1;
  static const int kDefaultPriority = 0;
  static const bool kNonDone = false;
};

// раз обрабатываем пачкой, то наверное нужны метки
// updated, to delete, ...
// Feature - пока нет
// Origin est., Current est., Elapsed, Remain
//
// TODO: нужно как-то сохранять со связями
//
// http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
class TaskEntity {
public:
  // ctor/dtor/assign/copy
  // FIXME: конструктор лучше закрыть
  TaskEntity();
  static boost::shared_ptr<TaskEntity> create(const std::string& task_name);
  static boost::shared_ptr<TaskEntity> create(const values::ImmutableTask& v);
  values::ImmutableTask make_value() const;
  //static values::ImmutableTask make_value(boost::shared_ptr<TaskEntity> e) const;

  // accessors
  int get_primary_key() const;
  void set_primary_key(int val);

  std::string get_task_name() const;
  void set_task_name(const std::string& value);

  int get_priority() const;
  void set_priority(const int val);

  bool get_is_done() const;
  void set_is_done(bool val);

private:
  //friend class pq_dal::TaskLifetimeQueries;  // только он меняет первичный ключ
  int id_;  // нужно какое-то не активное
  std::string task_name_;
  int priority_;
  bool is_done_;
};

// set лучше, но до сохранения индекс может быть не уникальным
typedef boost::shared_ptr<TaskEntity> TaskEntityPtr;
typedef boost::shared_ptr<const TaskEntity> ImmutableTaskEntityPtr;
typedef std::vector<entities::TaskEntityPtr> Tasks;
}  // namespace..

namespace values {
// FIXME: для таких объектов важно equal and hash!!
// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
//
// Придает семантику значений
// Ухудшает локальность кеша
// FIXME: Immutable now?
class ImmutableTask {
public:
    static ImmutableTask create();
    static ImmutableTask create(const std::string& d, const int p);
    static ImmutableTask create(const int id, const std::string& d, const int p);
    static ImmutableTask create(const int id, const std::string& d, const int p, const bool _done);

    // copy/assign
    ImmutableTask(const ImmutableTask& v);
    ImmutableTask& operator=(const ImmutableTask& v);

    // accessors
    int id() const;
    boost::shared_ptr<const std::string> description() const;
    int priority() const;
    bool done() const;

private:
    int _id;
    boost::shared_ptr<const std::string> _description;  // FIXME: NonImmutable really
    int _priority;
    bool _done;  // need store

    ImmutableTask(const int _id, const std::string& d, const int p, const bool);
};

// По идее это указание Виду снизу. Это плохо
struct TaskViewTableIdx {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};
}

namespace canary {
//std::set<std::string> labels_;  // TODO: сделать связь, но пока не нужно
//   weak_ptr<...>  - не должен владеть

// TODO: должны быть уникальные по имени и при создании это нужно контролировать.
// TODO: Как быть при обновлении имени или цвета? Возможно нужно хранить shared_ptrs не на константу.
class Tag {
public:
  Tag(const std::string& name) : primary_key_(), name_(name) { }

private:
  int primary_key_;
  std::string name_;
  std::string color_;
};
}

#endif // DOMAIN_H
