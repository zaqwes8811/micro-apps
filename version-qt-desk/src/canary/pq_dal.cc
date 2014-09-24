#include "top/config.h"

#include "canary/lower_level.h"
#include "canary/pq_dal.h"
#include "canary/entities.h"

// C++
#include <iostream>

// C
#include <cassert>

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
using domain::Model;

void TaskTableQueries::print(connection& conn) const {
  nontransaction no_tr_w(conn);
  string sql("SELECT * from " + table_name_ + ";");
  result r( no_tr_w.exec( sql ));

  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    cout << "ID = " << c[0].as<int>()
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

  lower_level::psql_space::run_transaction(sql, C);
}

void TaskTableQueries::drop(connection& C) {
  lower_level::psql_space::rm_table(C, table_name_);
}

void TaskLifetimeQueries::persist(
      Model tasks,
      pqxx::connection& C) 
  { 
  // FIXME: должно ли быть все атомарное
  //Model::iterator it = partion

  // Разбиваем на операции
  // save partion - no saved

  // update
}

void TaskLifetimeQueries::store(TaskEntity& task, connection& conn) {
  // нужно получить id
  // http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id
  string sql(
      "INSERT INTO " + table_name_ + " (task_name, priority) " \
      "VALUES ('"+task.get_task_name()+"', 32) RETURNING id; ");

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
  task.set_primary_key_(new_id);
}

domain::Model TaskLifetimeQueries::get_all(pqxx::connection& conn) const {
  work w(conn);
  string sql("SELECT * from " + table_name_ + ";");
  result r( w.exec( sql ));
  w.commit();

  Model model;
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    Model::value_type elem = TaskEntity::create("");
        //Model::value_type::element_type::create(string());
    elem->set_primary_key_(c[0].as<int>());
    elem->set_task_name(c[1].as<string>());
    elem->set_priority(c[2].as<int>());
    model.push_back(elem);
  }

  return model;
}

}

