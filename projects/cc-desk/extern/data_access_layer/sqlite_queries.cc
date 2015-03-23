#include "heart/config.h"

#include "sqlite_queries.h"

#include <string>
#include <iostream>

namespace sqlite_queries {
using std::string;
using entities::Task;
using entities::TaskEntities;
using sqlite3_cc::as;
using sqlite3_cc::sqlite3_exec;
using std_own_ext::at;
using entities::Tag;

// http://stackoverflow.com/questions/27764486/java-sqlite-last-insert-rowid-return-0
size_t get_last_id(const sqlite3_cc::Result& r) {
  // FIXME: Strange thing. Really return bunch identical rows.
  //DCHECK(r.size() == 1);

  size_t id(entities::EntityStates::kInactiveKey);
  for (auto& col : r) {
    id = as<size_t>(at(col, "last_insert_rowid()"));
    break;
  }

  DCHECK(id != entities::EntityStates::kInactiveKey);
  return id;
}

Task SQLiteTaskTableQueries::persist(const entities::Task& unsaved_task) {
  DCHECK(unsaved_task.id == entities::EntityStates::kInactiveKey);

  string sql(
      "INSERT INTO " + m_table_name + " (TASK_NAME, PRIORITY) " \
      "VALUES ('" + unsaved_task.name+"',"
      + std_own_ext::to_string(unsaved_task.priority) + "); " +
      "  SELECT last_insert_rowid() FROM " + m_table_name + "; ");

  auto r = exec(sql);

  auto saved_task = Task();
  saved_task.id = get_last_id(r);
  saved_task.name = unsaved_task.name;
  saved_task.priority = unsaved_task.priority;
  saved_task.done = false;
  return saved_task;
}

void SQLiteTaskTableQueries::update(const entities::Task& saved_task) {
  DCHECK(saved_task.id != entities::EntityStates::kInactiveKey);

  string done("false");
  if (saved_task.done) done = "true";

  string sql(
  "UPDATE "
        + m_table_name + " SET "
        + "TASK_NAME = '" + saved_task.name
        + "', PRIORITY = " + std_own_ext::to_string(saved_task.priority)
        + ", DONE = " + done
        + " WHERE ID = " + std_own_ext::to_string(saved_task.id) + ";");

  exec(sql);
}

TaskEntities SQLiteTaskTableQueries::loadAll() const {
  string sql("SELECT * FROM " + m_table_name + ";");

  auto r = exec(sql);

  TaskEntities tasks;
  for (auto& col : r) {
    Task saved_task;
    saved_task.id = as<size_t>(col["ID"]);
    saved_task.name = as<string>(col["TASK_NAME"]);
    saved_task.priority = as<int>(col["PRIORITY"]);
    saved_task.done = as<bool>(col["DONE"]);

    tasks.emplace_back(saved_task.share());
  }
  return tasks;
}

SQLiteTaskTableQueries::SQLiteTaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
                                               , const std::string& tableName)
  : m_table_name(tableName), m_connPtr(h)
{ }

void SQLiteTaskTableQueries::registerBeanClass() {
  std::string sql(
    "CREATE TABLE IF NOT EXISTS "+
    m_table_name +
    "(" \
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE       BOOLEAN            DEFAULT FALSE);");

  exec(sql);
}

void SQLiteTaskTableQueries::drop() { exec("DROP TABLE " + m_table_name + ";"); }

bool SQLiteTagTableQuery::checkUnique(const std::string& name) {
  auto sql = "SELECT NAME FROM " + m_table_name + " WHERE NAME ='" + name + "';";
  return exec(sql).empty();
}

Tag SQLiteTagTableQuery::persist(const Tag& unsaved_tag) {
  using sqlite3_cc::operator <<;

  DCHECK(unsaved_tag.id == entities::EntityStates::kInactiveKey);
  DCHECK(checkUnique(unsaved_tag.name));

  string sql(
      "INSERT INTO " + m_table_name + " (NAME, COLOR) " \
      "VALUES ('" + unsaved_tag.name+"','" + unsaved_tag.color + "'); " \
      "  SELECT last_insert_rowid() FROM " + m_table_name + "; ");

  auto r = exec(sql);

  auto saved_tag = Tag();
  saved_tag.id = get_last_id(r);
  saved_tag.name = unsaved_tag.name;
  saved_tag.color = unsaved_tag.color;

  return saved_tag;
}


SQLiteTagTableQuery::SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h)
    : m_table_name(models::s_kTagTableName) , m_connPtr(h)
{ }

void SQLiteTagTableQuery::registerBeanClass()  {
  // http://www.tutorialspoint.com/sqlite/sqlite_using_autoincrement.htm
  std::string sql =
      "CREATE TABLE IF NOT EXISTS " + m_table_name + "("  \
           "ID             INTEGER  PRIMARY KEY     AUTOINCREMENT," \
           "NAME           TEXT                    NOT NULL," \
           "COLOR          TEXT                    NOT NULL);";

  exec(sql);
}

void SQLiteTagTableQuery::drop() {
  exec("DROP TABLE " + m_table_name + ";");
}
}  // space
