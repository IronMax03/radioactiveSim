#include "Quadtree.h"

template<typename T>
Quadtree<T>::Quadtree(){}

template<typename T>
LazyNode<T>& Quadtree<T>::get_root() const { return this->root;}
