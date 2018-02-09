#ifndef DELETER_H
#define DELETER_H

// see http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Checked_delete
// The C++ Standard allows, in 5.3.5/5, 
// pointers to incomplete class types to be deleted with a delete-expression. 
// When the class has a non-trivial destructor, or a class-specific operator delete, 
// the behavior is undefined, meaning the non-trivial behavior might not occur
class to_be_deleted;
//#include "to_be_deleted.h"

class deleter { 
public:
  void delete_it(to_be_deleted* p);
};

#endif //DELETER_H