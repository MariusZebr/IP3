#include "Product.h"
#include <vector>

class ProductContainer
{
private:
  class Impl;
  Impl *pImpl;

public:
  ProductContainer();
  ~ProductContainer();

  void setPricingStrategy(PricingStrategy *s);
  double recalculatePrice(int index) const;

  class ForwardIterator
  {
  public:
    class IteratorImpl;

  private:
    IteratorImpl *pIterImpl;

  public:
    ForwardIterator(IteratorImpl *impl);
    ForwardIterator(const ForwardIterator &other);
    ~ForwardIterator();

    ForwardIterator &operator=(const ForwardIterator &other);
    Product *&operator*();
    ForwardIterator &operator++();
    ForwardIterator operator++(int);

    bool operator!=(const ForwardIterator &other) const;
    bool operator==(const ForwardIterator &other) const;
  };

  // CRUD operations

  // Create
  void add(Product *p);

  // Read
  Product *get(int index) const;

  // Update
  void update(int index, Product *p);

  // Delete
  void remove(int index);

  // Iterator methods
  ForwardIterator begin();
  ForwardIterator end();
};
