#include "Product.h"
#include <vector>

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

  class ForwardIterator
  {
  // to allow ProductContainer to access private ForwardIteratorImpl constructor
  friend class ProductContainer;
  private:
    class IteratorImpl;
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

  void setPricingStrategy(PricingStrategy *s);
  double recalculatePrice(ForwardIterator &position) const;

  // CRUD operations

  void push_back(Product *p);
  void insert(ForwardIterator &position, Product *p);
  Product *get(const ForwardIterator &position) const;
  void update(const ForwardIterator &position, Product *p);
  void remove(std::vector<Product *>::iterator position);
  
  // Iterator methods
  ForwardIterator begin();
  ForwardIterator end();
};
