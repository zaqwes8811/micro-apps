#include "heart/config.h"

#include "arch.h"
#include "view/qt_event_loop.h"

#include <boost/lexical_cast.hpp>

// http://www.cplusplusdevelop.com/829_19589028/

std::shared_ptr<SingleWorker> ExecutionContexts::s_dbWorker(new SingleWorker);
//gc::SharedPtr
std::shared_ptr
<actors::UIActor> ExecutionContexts::s_ui_actor(new actors::UIActor);
//= std::make_shared<actors::UIActor>();

void ExecutionContexts::Post(Ids id, SingleWorker::Callable fun) {
  if (DB == id) {
    auto p = get_db().lock();
    if (!p)
      throw std::runtime_error(FROM_HERE);
    p->post(fun);
  } else if (UI == id) {
    auto p = get_ui().lock();
    if (!p)
      throw std::runtime_error(FROM_HERE);
    p->Post(fun);
  } else { }
}

void ExecutionContexts::ForkAll() {
  auto p = get_ui().lock();
  if (!p)
    throw std::runtime_error(FROM_HERE);
  p->Fork();
}

void ExecutionContexts::JoinAll() {
  auto p = get_ui().lock();
  if (!p)
    throw std::runtime_error(FROM_HERE);
  p->Join();
}

std::string SingleWorker::getCurrentThreadId() {
  return boost::lexical_cast<std::string>(std::this_thread::get_id());
}

std::string SingleWorker::GetId() {
  auto work = SingleWorker::getCurrentThreadId;
  std::packaged_task<std::string()> t(work);
  std::future<std::string> f = t.get_future();

  SingleWorker::Callable pkg
      = std::bind(&std::packaged_task<std::string()>::operator(), std::ref(t));
  post(pkg);

  return f.get();
}

std::string ExecutionContexts::decodeId(Ids id) {
  std::map<int, std::string> m;
  m[DB] = dbId();

  if (m.find(id) == m.end())
    throw std::runtime_error(FROM_HERE);

  return m[id];
}

std::future<int> ExecutionContexts::ActivateUiEventLoop(concepts::db_queries_generator_concept_t db) {
  auto p = get_ui().lock();
  if (!p)
    throw std::runtime_error(FROM_HERE);
  return p->RunUI(db);
}

std::string ExecutionContexts::dbId() {
  auto p = get_db().lock();
  if (!p)
    throw std::runtime_error(FROM_HERE);
  return p->GetId();
}

