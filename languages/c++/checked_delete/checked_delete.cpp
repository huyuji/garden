#include "deleter.h"
#include "to_be_deleted.h"

void testCheckedDelete() {
  to_be_deleted* p = new to_be_deleted;

  deleter d;
  d.delete_it(p);
}