#pragma once

#include <nitro.h>

template<typename T>
class TArray
{
    public:

    TArray()
    {
        mBegin = NULL;
        mEnd = NULL;
        mCap = NULL;
    }

    TArray(u32 n)
    {
        mBegin = NULL;
        mEnd = NULL;
        mCap = NULL;
        
        reserve(n);
    }
    ~TArray();

    void reserve(u32 n);
    void resize(u32 n, T const & v = T());
    void shrink();

    void clear() { mEnd = mBegin; }
    T * insert(T const * it, T const & v, u32 n = 1);
    void push(T const & v) { insert(end(), v); }
    void pop() { erase(end() - 1); }
    T * erase(T * it);
    T * erase(T * first, T * last);

    u32 capacity() const {
        return static_cast<u32>(mCap - mBegin);
    }

    u32 size() const {
        return static_cast<u32>(mEnd - mBegin);
    }

    T & operator[](u32 i) { return *(mBegin + i); }
    T const & operator[](u32 i) const { return *(mBegin + i); }

    T * begin() { return mBegin; }
    T const * begin() const { return mBegin; }
    T const * cbegin() const { return mBegin; }

    T * end() { return mBegin; }
    T const * end() const { return mBegin; }
    T const * cend() const { return mBegin; }

    T * data() { return mBegin; }
    T const * data() const { return mBegin; }

    private:

    T * mBegin;
    T * mEnd;
    T * mCap;
};

template<typename T>
void TArray<T>::reserve(u32 n) {
  if (capacity() >= n) {
    return;
  }

  u32 old_size = size();
  T * begin = new T[n];

  for (u32 i = 0; i < old_size; ++i) {
    begin[i] = mBegin[i];
  }

  delete[] mBegin;
  mBegin = begin;
  mEnd = (mBegin + old_size);
  mCap = (mBegin + n);
}

template<typename T>
TArray<T>::~TArray() {
  delete[] mBegin;
}

// -------------------------------------------------------------------------- //

template<typename T>
void TArray<T>::resize(u32 n, T const & v) {
  if (size() > n) {
    erase((begin() + n), end());
  } else if (size() < n) {
    insert(end(), v, (n - size()));
  }
}

// -------------------------------------------------------------------------- //

template<typename T>
void TArray<T>::shrink() {
  u32 n = size();

  if (capacity() == n) {
    return;
  }

  T * begin = new T[n];

  for (u32 i = 0; i < n; ++i) {
    begin[i] = mBegin[i];
  }

  delete[] mBegin;
  mBegin = begin;
  mCap = mEnd = (mBegin + n);
}

// -------------------------------------------------------------------------- //

template<typename T>
typename TArray<T>::T*
TArray<T>::insert(T const * it, T const & v, u32 n) {
  u32 a = static_cast<u32>(it - mBegin);

  if (n == 0) {
    return (mBegin + a);
  }

  u32 b = static_cast<u32>(a + n);
  u32 old_size = size();
  u32 new_size = (old_size + n);
  u32 m = (old_size - a);

  if (capacity() >= new_size) {
    T const * pv = &v;

    if (it <= pv && pv < mEnd) {
      pv += n;
    }

    for (u32 i = 0; i < m; ++i) {
      mBegin[b + m + n - (i + 1)] = mBegin[b + m - (i + 1)];
    }

    for (u32 i = 0; i < n; ++i) {
      mBegin[a + i] = *pv;
    }

    mEnd += n;
  } else {
    T * begin = new T[new_size];

    for (u32 i = 0; i < a; ++i) {
      begin[i] = mBegin[i];
    }

    for (u32 i = 0; i < m; ++i) {
      begin[b + i] = mBegin[a + i];
    }

    for (u32 i = 0; i < n; ++i) {
      begin[a + i] = v;
    }

    delete[] mBegin;
    mBegin = begin;
    mEnd = (mBegin + new_size);
    mCap = (mBegin + new_size);
  }

  return (mBegin + b);
}

// -------------------------------------------------------------------------- //

template<typename T>
typename TArray<T>::T*
TArray<T>::erase(T * it) {
  return erase(it, (it + 1));
}

// -------------------------------------------------------------------------- //

template<typename T>
typename TArray<T>::T*
TArray<T>::erase(T * first, T * last) {
  if (first == last) {
    return const_cast<T *>(last);
  }

  if (first == mBegin && last == mEnd) {
    clear();
    return end();
  }

  u32 a = static_cast<u32>(first - mBegin);
  u32 b = static_cast<u32>(last - mBegin);
  u32 n = (size() - b);

  for (u32 i = 0; i < n; ++i) {
    mBegin[a + i] = mBegin[b + i];
  }

  mEnd -= (b - a);
  return (mBegin + a);
}

// -------------------------------------------------------------------------- //