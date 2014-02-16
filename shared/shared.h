
#ifndef __SHARED_H_
#define __SHARED_H_

#include <QMutex>
#include <QQueue>

// to be used by a single consumer
//
template <class T>
class ConcurrentQueue
{
  QMutex m_Lock;
  QQueue<T> m_Queue;
public:
  bool empty() {
      QMutexLocker locker(&m_Lock);
      return m_Queue.isEmpty();
  }

  void push(T item) {
      QMutexLocker locker(&m_Lock);
      m_Queue.enqueue(item);
  }

  T pop() {
      QMutexLocker locker(&m_Lock);
      return m_Queue.dequeue();
  }
};

#endif // __SHARED_H_
