#ifndef GLOBALCONTEXT_H
#define GLOBALCONTEXT_H

#include <QObject>


class GlobalContextObserver
{
public:
    virtual void contextChanged() = 0;
};

class GlobalContext
{
private:
     static int _currentSubjectId;
     static QList<GlobalContextObserver*> _observers;
private:
     static void notifyObservers()
     {
         GlobalContextObserver* o;
         foreach(o, _observers)
         {
             o->contextChanged();
         }
     }
public:
    //static GlobalContext* getInstance();
    static int getCurrentSubjectId() { return _currentSubjectId; }
    static void setCurrentSubjectId(int newId) { _currentSubjectId = newId; notifyObservers(); }

    static void addObserver(GlobalContextObserver* observer)
    {
        if(observer && !_observers.contains(observer))
        {
            _observers.append(observer);
        }
    }

    static void removeObserver(GlobalContextObserver* observer)
    {
        int index = _observers.indexOf(observer);
        if(index >= 0)
        {
            _observers.removeAt(index);
        }
    }
};

#endif // GLOBALCONTEXT_H
