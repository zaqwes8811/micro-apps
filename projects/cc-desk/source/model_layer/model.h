#ifndef BUSI_H
#define BUSI_H

#include "model_layer/entities.h"
#include "data_access_layer/postgresql_queries.h"
#include "view/renders.h"
#include "model_layer/model.h"
#include "model_layer/isolation.h"
//#include "core/actor_ui.h"

#include <string>

namespace actors {
  class UIActor;
}

namespace models
{
/**
  \fixme как вообще работать с кешем и базами данных.
  в кешах ограниченное api!
  http://en.wikipedia.org/wiki/Database_caching - есть про когерентность.

  But it need really?
  http://stackoverflow.com/questions/548301/what-is-caching
  http://stackoverflow.com/questions/2916645/implementing-model-level-caching?rq=1
  http://stackoverflow.com/questions/343899/how-to-cache-data-in-a-mvc-application?rq=1
  domain::TasksMirror store_cache_;
  bool miss_;  // кеш устарел

  FIXME: логичекая проблема с фильтрами - как быть, если каждый раз не перезагужать кеш?
  похоже есть зависимость от текущего фильтра. А если отред. и теперь в фильтр не попадает?

  FIXME: утекают хендлы!! make ImmutableTask. причем утекают как на нижние уровни, так и на верхние
*/
class Model
    //: public boost::noncopyable
{
public:
  // create/destory
  static Model* createForOwn(storages::DataBasePtr);
  Model(entities::TaskEntities _model, storages::DataBasePtr _pool);
  ~Model();

  // other
  // FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  void setListener(isolation::ModelListenerPtr iso);

  /**

    \fixme наверное лучше сразу сохранить, добавлять все равно буду скорее всего по-одному
  */
  void appendNewTask(const entities::Task& e);  // overloading trouble in for_each

  // Precond: элемент был сохранен
  void update(const entities::Task& e);

  // FIXME: плохо что хендлы утекают, и из-за того что указатели
  //   shared объекты превращаются в глобальные переменные.
  entities::TaskEntities getCurrentModelData();

  void dropStore();

  void setUiActor(gc::SharedPtr<actors::UIActor> a)
  { m_uiActorPtr = a; }

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);
  void draw_task_store(std::ostream& o) const;

  void notify();  // Нужно было открыть для обновления при семене фильтров

  // persist filters:
  static entities::TaskEntities load_all(storages::DataBasePtr pool);

  entities::TaskEntities m_tasks;

  // FIXME: кажется двойное лучше, или хранить фильтр? и через него при прорисовке пропускать?
  storages::DataBasePtr m_dbPtr;
  isolation::ModelListenerPtr m_observersPtr;
  entities::TaskEntity getElemById(const size_t id);

  gc::SharedPtr<actors::UIActor> m_uiActorPtr;
};
}

#endif // BUSI_H
