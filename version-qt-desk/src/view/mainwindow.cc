// draw one row
// FIXME: лучше это сконнектить!! операция логически неделимая
//   в принципе можно и слот на изменение сделать один.
//
// id
// FIXME: так лучше не хранить, но как быть? Как надежно хранить соответствие?
//   в объект не включить, разве что можно сделать нисходящее преобразование
//   хотя похоже это тоже не выход. Итого. Где хранить ключ!?
//
//vector<QTableWidget*> tmp;
//tmp.push_back();  // если будет исключение, то будет утечка памяти
// мы во владение не передали

#include "top/config.h"

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_help_data.h"
#include "canary/entities_and_values.h"

//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <boost/bind.hpp>
#include <adobe/algorithm/for_each.hpp>

#include <string>
#include <vector>

using app_core::Model;
using entities::Tasks;  // not work
using values::TaskTableIdx;

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

QTableWidgetCheckEdited::QTableWidgetCheckEdited(QWidget *parent)
    : QTableWidget(parent) {

  // fill lists
  s_column_names_.append("id");
  s_column_names_.append("name");
  s_column_names_.append("priority");

  setColumnCount(s_column_names_.size());
  setHorizontalHeaderLabels(s_column_names_);
  setColumnHidden(TaskTableIdx::kId, true);  // FIXME: id's пока так
}

View::View(app_core::Model* const app_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app_ptr_(app_ptr)
{
  ui->setupUi(this);

  // table
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  // control
  QPushButton* submit = new QPushButton("Sort by decrease priority", this);
  connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotSortByDecreasePriority(bool)));

  QPushButton* mark_done = new QPushButton("Mark done", this);
  //connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  scoreTable_ = new QTableWidgetCheckEdited(this);
  connect(scoreTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

  QVBoxLayout* actions_layout = new QVBoxLayout;//(mainLayout);
  actions_layout->addWidget(submit);
  actions_layout->addWidget(mark_done);

  mainLayout->addLayout(actions_layout);
  mainLayout->addWidget(scoreTable_);

  // Add empty lines - нужно загрузить старые, но как
  updateAction();
}

View::~View()
{
    delete ui;
}

void View::insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int row = end; row < end+app_core::kAddedBlankLines; ++row) {
      scoreTable_->setItem(row, values::TaskTableIdx::kId,
                           new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey)));
      scoreTable_->setItem(row, values::TaskTableIdx::kTaskName,
                           new QTableWidgetItem);
      scoreTable_->setItem(row, values::TaskTableIdx::kPriority,
                           new QTableWidgetItem);
  }
}

void View::clearList() {
  // есть и функция clear and clearContent

  int count_rows = scoreTable_->rowCount();
  for (int i = 0; i < count_rows; ++i)
    scoreTable_->removeRow(i);
}

void View::slotSortByDecreasePriority(bool checked) {
  //Tasks mirror(test_help_data::build_fake_model());

  // сохраняем все
  //adobe::for_each(mirror, bind(&Model::append, ref(*app_ptr_), _1));
}

void View::updateAction() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  clearList();

  Tasks records = app_ptr_->get_current_model_data();

  {
    // fill table
    scoreTable_->setRowCount(records.size() + app_core::kAddedBlankLines);

    int row = 0;
    for (Tasks::const_iterator record=records.begin(), end=records.end(); record != end; ++record) {
      scoreTable_->setItem(row, values::TaskTableIdx::kId,
                           new QTableWidgetItem(QString::number((*record)->get_primary_key())));
      scoreTable_->setItem(row, values::TaskTableIdx::kTaskName,
                           new QTableWidgetItem(QString::fromUtf8((*record)->get_task_name().c_str())));
      scoreTable_->setItem(row, values::TaskTableIdx::kPriority,
                           new QTableWidgetItem(QString::number((*record)->get_priority())));

      ++row;
    }

    // вставляем еще несколько рядов
    insertBlankRows(row);
  }
}

void View::slotRowIsChanged(QTableWidgetItem* elem)
{
  using values::TaskValue;
  using entities::EntitiesStates;

  // FIXME: проблема!! изменения любые! может зациклить
  // FIXME: а такая вот комбинация надежно то работает?
  if (scoreTable_->edited()) {
    // надежнее всего получить ID строки, индексу я не верю.
    //   может через downcasting? RTTI in Qt кажется отключено
    // http://codebetter.com/jeremymiller/2006/12/26/downcasting-is-a-code-smell/
    int id = scoreTable_->item(elem->row(), values::TaskTableIdx::kId)->text().toInt();

    if (id == EntitiesStates::kInActiveKey) {
      // создаем новую запись
      QString d(
          scoreTable_->item(elem->row(), values::TaskTableIdx::kTaskName)->text());

      QString priority(
          scoreTable_->item(elem->row(), values::TaskTableIdx::kPriority)->text());

      if (d.isEmpty() && priority.isEmpty())
        return;

      int p(entities::EntitiesStates::kDefaulPriority);
      if (!priority.isEmpty())
        p = priority.toInt();

      // FIXME: no injection bad!
      TaskValue v(TaskValue::create(d.toUtf8().constData(), p));
      app_ptr_->append_value(v);
    } else {
      // просто обновляем
      Tasks::value_type e(app_ptr_->get_elem_by_pos(elem->row()));

      assert(id == e->get_primary_key());

      QString d(scoreTable_->item(elem->row(), values::TaskTableIdx::kTaskName)->text());
      int p(scoreTable_->item(elem->row(), values::TaskTableIdx::kPriority)->text().toInt());

      e->set_priority(p);
      e->set_task_name(d.toUtf8().constData());

      // обновляем
      app_ptr_->update(e);
    }
  }
}
