#include "globalcontext.h"

int GlobalContext::_currentSubjectId = -1;
QList<GlobalContextObserver*> GlobalContext::_observers;
