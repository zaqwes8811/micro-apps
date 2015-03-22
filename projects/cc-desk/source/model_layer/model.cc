#include "heart/config.h"

#include "model_layer/model.h"
#include "model_layer/filters.h"
#include "core/actor_ui.h"

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

extern gc::SharedPtr<actors::UIActor> gUIActor;  // dtor will call and app out
extern gc::SharedPtr<cc11::Actior> gDBActor;


namespace models {
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using entities::Task;

using std::begin;
using std::end;
using std::cout;

namespace ext {
void onNew(gc::SharedPtr<Model> m, TaskEntity e, Task t) {
  *e = t;
  auto iter =
      std::find_if(begin(m->m_taskCells), end(m->m_taskCells),
                   [e] (models::Model::TaskCell v) { return e->id == v.second->id; });

  if (iter == end(m->m_taskCells)) return;

  iter->first = true; // off lock
  m->notifyObservers();
}
}

void Model::dropStore() {
  auto db = m_db;

  gDBActor->post([db]() {
    auto q = db->getTaskTableQuery();
    concepts::drop(q);
  });
}

void Model::initialize(std::function<void(std::string)> errorHandler) {

  auto onLoaded = [this](entities::TaskEntities tasks) {
    m_taskCells.clear();
    for (auto& task : tasks)
      m_taskCells.emplace_back(true, task);
    notifyObservers();
  };

  auto db = m_db;
  gDBActor->post([onLoaded, db] {
    auto q = db->getTaskTableQuery();
    auto query = db->getTaskLifetimeQuery();

    concepts::registerBeanClass(q);
    auto tasks = query.loadAll();
    gUIActor->post(std::bind(onLoaded, tasks));
  });
}

Model::TaskCell Model::getCachedTaskById(const size_t id) {
  auto iter = std::find_if(
        m_taskCells.begin(), m_taskCells.end(),
        [id] (const TaskCell& elem) -> bool {
          return filters::is_contained(id)(elem.second);
        });

  DCHECK(iter != m_tasksCache.end());
  return *iter;
}

void Model::updateTask(const entities::Task& e) {
  auto k = getCachedTaskById(e.id);

  // Is locked?
  if (!k.first) {
    return;
  } else {
    // Prepare
    auto q = m_db->getTaskLifetimeQuery();

    // Action
    q.update(k.second->toValue());

    // .then()
    notifyObservers();
  }
}

void Model::addFilter(filters::FilterPtr f)
{ m_filtersChain.add(f); notifyObservers(); }

void Model::removeFilter(filters::FilterPtr f)
{ m_filtersChain.remove(f); notifyObservers(); }

// FIXME: may be not put in RAM? After persist view will be updated
//} catch (...) {
  // No way! Can add some task after
  //auto rollback = [this]() {
    //std::remove()
    // FIXME: No can't. Lost user input!
    //this->m_taskCells.pop_back();  // no way!
  //};
  //rollback();
//}
// FIXME: if somewhere failed after persist - then.. state is protect?
void Model::appendNewTask(const Task& unsaved_task) {
  using std::find_if;
  using namespace std::placeholders;

  // FIXME: show error message
  DCHECK(unsaved_task.id == EntityStates::kInactiveKey);

  // on lock
  auto e = unsaved_task.toEntity();
  m_taskCells.push_back({false, e});

  auto unlock = std::bind(&ext::onNew, shared_from_this(), e, _1);

  auto db_ptr = m_db;
  gDBActor->post([unsaved_task, unlock, db_ptr] () mutable {
    auto query = db->getTaskLifetimeQuery();
    auto new_task = query.persist(unsaved_task);
    gUIActor->post(std::bind(unlock, new_task));
  });
}

entities::TaskEntities Model::filterModelData() {
  auto r = entities::TaskEntities();
  std::transform(begin(m_taskCells), end(m_taskCells),
                 std::back_inserter(r),
                 [](TaskCell cell) -> entities::TaskEntity {
                    return cell.second;});

  return m_filtersChain(r);
}

Model::Model(concepts::db_manager_concept_t _pool)
  : m_db(std::make_shared<concepts::db_manager_concept_t>(_pool)) { }
void Model::notifyObservers() { m_observersPtr->update(filterModelData()); }
void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso) { m_observersPtr = iso; }

}
