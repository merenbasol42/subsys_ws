// Pair.h
#ifndef PAIR_H
#define PAIR_H

template<typename T>
struct Pair {
    T left;
    T right;
    
    // İsteğe bağlı: parametre alan constructor ekleyebilirsiniz.
    Pair() = default;
    Pair(T l, T r) : left(l), right(r) {}
    void eq(Pair<T> p) {
        left = p.left;
        right = p.right;
    }
};

#endif // PAIR_H
