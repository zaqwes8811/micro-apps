// Caching
//   http://www.postgresql.org/message-id/E16gYpD-0007KY-00@mclean.mail.mindspring.net

#include "top/config.h"

#include "canary/pq_queries.h"
#include "canary/entities.h"
#include "canary/filters.h"

#include <boost/bind.hpp>
#include <boost/bind/make_adaptable.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <adobe/algorithm/partition.hpp>

#include <iostream>
#include <cassert>
#include <sstream>

namespace common {
template <typename T>
std::string to_string(T const& value) {
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}
}

namespace pq_dal {
using namespace boost;

using std::string;
using std::cout;
using std::endl;
using pqxx::connection;
using pqxx::work;
using pqxx::result;
using pqxx::nontransaction;
using domain::TaskEntity;
using domain::TasksMirror;
using domain::EntitiesStates;

PQConnectionPool::PQConnectionPool(const std::string& conn_info)
  : conn_(new pqxx::connection(conn_info)) {
  assert(conn_->is_open());
}

PQConnectionPool::~PQConnectionPool() {
  try {
    conn_->disconnect();
  } catch (...) {}
}


void TaskTableQueries::print(std::ostream& o, connection& conn) const {
  nontransaction no_tr_w(conn);
  string sql("SELECT * from " + table_name_ + " order by id;");
  result r( no_tr_w.exec( sql ));

  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    o << "ID = " << c[0].as<int>()
         << " Name = " << c[1].as<string>()
         << " Priority = "  << c[2].as<int>() << endl;
  }
}

void TaskTableQueries::createIfNotExist(connection& C) {
  string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+ // v9.1 >=
    table_name_ +
    "(" \
    // сделать чтобы было >0
    "id         SERIAL PRIMARY KEY NOT NULL," \
    "task_name  TEXT               NOT NULL, " \
    "priority   INT                NOT NULL);");

  pq_lower_level::run_transaction(sql, C);
  //pq_lower
}

void TaskTableQueries::drop(connection& C) {
  pq_lower_level::rm_table(C, table_name_);
}

void TaskLifetimeQueries::update(domain::TasksMirror::value_type e, pqxx::connection& C) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  string sql(
  "UPDATE "
        + task_table_name_ + " SET "
        + "task_name = '" + e->get_task_name()
        + "', priority = " + common::to_string(e->get_priority())
        + " WHERE id = " + common::to_string(e->get_primary_key()) + ";");
  
  work w(C);
  w.exec(sql);
  w.commit();
}

void TaskLifetimeQueries::create(domain::TasksMirror::value_type task, pqxx::connection& C) {
  create(*task, C);
}

void TaskLifetimeQueries::create(
      TasksMirror tasks,
      pqxx::connection& conn)
  { 



  // FIXME: должно ли быть все атомарное
  TasksMirror::iterator it = adobe::stable_partition(tasks, filters::get_check_non_saved());

  if (it != tasks.begin()) {
    assert(std::distance(tasks.begin(), it) < 100);

    string sql("INSERT INTO " + task_table_name_ + " (task_name, priority) VALUES");
    for (TasksMirror::const_iterator at = tasks.begin(); ;) {
      sql += "('"
        + (*at)->get_task_name()  // будут проблемы с юникодом
        + "', "  
        + common::to_string((*at)->get_priority())
        + ")";

      ++at;
      if (at == it)
        break;

      sql += ", ";
    }

    sql += " RETURNING id;";

    // make query
    work w(conn);
    result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
    w.commit();

    {
      int i = 0;
      for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
        int new_id(domain::EntitiesStates::kInActiveKey);
        new_id = c[0].as<int>();
        assert(new_id != domain::EntitiesStates::kInActiveKey);
        tasks[i]->set_primary_key_(new_id);

        ++i;
      }
    }
  }

  // Разбиваем на операции
  // save partion - no saved

  // update
}

void TaskLifetimeQueries::create(
    domain::TasksMirror::value_type::element_type& e, connection& conn) {
  // нужно получить id
  // http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id
  string sql(
      "INSERT INTO " + task_table_name_ + " (task_name, priority) " \
        "VALUES ('"
        + e.get_task_name()+"', "
        + common::to_string(e.get_priority()) + ") RETURNING id; ");

  work w(conn);
  result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
  w.commit();

  // Узнаем что за ключ получили
  int new_id(domain::EntitiesStates::kInActiveKey);

  assert(r.size() == 1);  // вставили один элемент
  
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    new_id = c[0].as<int>();
    break;
  }

  assert(new_id != domain::EntitiesStates::kInActiveKey);
  e.set_primary_key_(new_id);
}

domain::TasksMirror TaskLifetimeQueries::get_all(pqxx::connection& conn) const {
  work w(conn);
  string sql("SELECT * from " + task_table_name_ + ";");
  result r( w.exec( sql ));
  w.commit();

  TasksMirror model;
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    TasksMirror::value_type elem = TaskEntity::create("");
        //Model::value_type::element_type::create(string());
    elem->set_primary_key_(c[0].as<int>());
    elem->set_task_name(c[1].as<string>());
    elem->set_priority(c[2].as<int>());
    model.push_back(elem);
  }

  return model;
}

}  // ns

namespace pq_lower_level {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(connection& conn, const string& table_name)
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql("drop table " + table_name + ";");

  // создаем транзакционный объект
  work w(conn);

  // Exec
  w.exec(sql);
  w.commit();
}

void run_transaction(const string& sql, /*const*/ connection& C)
{
  work W(C);
  W.exec(sql);
  W.commit();
}
}  // ns


