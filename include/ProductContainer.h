#ifndef PRODUCT_CONTAINER_H_832035
#define PRODUCT_CONTAINER_H_832035

#include <functional>
#include <vector>
#include "Product.h"
#include "../include/strategies/PricingStrategy.h"
#include "../include/exceptions/StrategyNotSetException.h"

class ProductContainer
{
private:
  class Impl;
  Impl *pImpl;

public:
  ProductContainer();
  ProductContainer(const ProductContainer &other);
  ~ProductContainer();
  ProductContainer &operator=(const ProductContainer &other);
  
  void forEach(std::function<void(Product *)> callback);
  std::string listProducts() const;

  class ForwardIterator
  {
  // to allow ProductContainer to access private class IteratorImpl
  friend class ProductContainer;
  private:
    class IteratorImpl;
    IteratorImpl *pIterImpl;
    
  public:
    ForwardIterator(IteratorImpl *impl);
    ForwardIterator(const ForwardIterator &other);
    ~ForwardIterator();
    ForwardIterator &operator=(const ForwardIterator &other);

    // everything below is required for a forward iterator
    using iterator_category = std::forward_iterator_tag;
    using value_type        = Product*;
    using difference_type   = std::ptrdiff_t;
    using pointer           = Product**;
    using reference         = Product*&;
    Product *&operator*();
    ForwardIterator &operator++();
    ForwardIterator operator++(int);

    bool operator!=(const ForwardIterator &other) const;
    bool operator==(const ForwardIterator &other) const;
  };

  void setPricingStrategy(PricingStrategy *s);
  double recalculatePrice(ForwardIterator &position) const;

  // CRUD operations

  void push_back(Product *p);
  void insert(ForwardIterator position, Product *p);
  Product *get(const ForwardIterator position) const;
  void update(const ForwardIterator position, Product *p);
  void remove(ForwardIterator position);
  
  // Iterator methods
  ForwardIterator begin() const;
  ForwardIterator end() const;

  int getSize() const;
  void clear();

  std::string toString() const;
};

#endif // PRODUCT_CONTAINER_H_832035

