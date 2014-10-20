// WARNING: danger! всю иерархию фильтров хранить в этой паре файлов! это связано в операцией
//   сравнения по типу.
//
// TROUBLE: фильтры не такие простные штуки
//
// http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
// лучше hash_set, set как-то странно проверяет на равенство
// можно hash_map, может так и проще, тогда не нужны свои hash and equal
//
// Logic fail:
//   stable_sort это не то же самое, что отсортировать... - главное, что при пересорт. одинак. элем. сохр. порядок
//   скорее всего дело в том, что строки !!не одинаковые - у них одинаковое начало, но не сами они
//
// Feature:
//   отсортировать по имени и в группах одинаковых по приоритету

#ifndef FILTERS_H
#define FILTERS_H

#include "canary/entities_and_values.h"

#include <boost/function.hpp>
#include <boost/unordered_set.hpp>

#include <list>

namespace filters
{

boost::function1<bool, entities::Tasks::value_type> get_check_non_saved();
boost::function1<bool, entities::Tasks::value_type> get_check_contained(const int id);

// могли бы вставляться друг в друга
class Filter {
public:
  // ctors/..
  virtual ~Filter() { }
  virtual entities::Tasks operator()(entities::Tasks t) = 0;

  // FIXME: хочется обойтись без rtti, пока так.
  //   Проблема в удалении фильтра из цепочки.
  virtual int get_type_code() const = 0;
};
typedef boost::shared_ptr<Filter> FilterPtr;

// сырые указатели лучше не передавать.
bool operator==(const Filter& lhs, const Filter& rhs);
bool operator==(FilterPtr lhs, FilterPtr rhs);
//std::size_t hash_value(FilterPtr b);  // пока страшновато, до конца не понял

struct KeyHasher
{
  std::size_t operator()(FilterPtr k) const
  {
    using boost::hash;
    return hash<int>()(k->get_type_code());
  }
};

struct KeyEqual {
    bool operator()(FilterPtr lhs, FilterPtr rhs) const {
        return lhs == rhs;
    }
};

// на входе весь кеш, на выходе результат собственно, может лучше через SQL?
// Но как легко комбинировать фильтры. Откат к sql может повлиять, а может и нет на архитектуры.
//
// зацепаемся за типы? может еще можно как-то?
// FIXME: Фильтры должны быть линейными, а значить переставляемыми, но как доказать?
class ChainFilters {
public:
  ChainFilters();
  void add(FilterPtr e);

  // FIXME: как удалить то без RTTI? Список то полиморфный
  void remove(FilterPtr e);

  entities::Tasks operator()(entities::Tasks e) const;

private:
  // FIXME: можно вообще тупо массив, фильтров все равно не 100500
  boost::unordered_set<FilterPtr, KeyHasher, KeyEqual> s_;  // need own hasher
};

class DoneFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e);
  int get_type_code() const;
};

class SortByPriorityFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e);
  int get_type_code() const;
};

class SortByTaskName : public Filter {
public:
    entities::Tasks operator()(entities::Tasks e);
    int get_type_code() const;
};
}

#endif // FILTERS_H
