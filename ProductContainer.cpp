#include "Product.h"
#include "PricingStrategy.h"
#include "ProductContainer.h"
#include "StrategyNotSetException.h"
#include <vector>
#include <algorithm>

class ProductContainer::Impl
{
private:
  // fields
  std::vector<Product *> data;
  PricingStrategy *pricingStrategy;

public:
  Impl();
  ~Impl();
  Impl *clone() const;

  void forEach(std::function<void(Product *)> callback);

  void setPricingStrategy(PricingStrategy *s);
  double recalculatePrice(ForwardIterator &position) const;

  // CRUD operations

  void push_back(Product *p);
  void insert(ForwardIterator &position, Product *p);
  Product *get(const ForwardIterator &position) const;
  void update(const ForwardIterator &position, Product *p);
  void remove(std::vector<Product *>::iterator position);

  // Iterator methods
  std::vector<Product *>::iterator begin();
  std::vector<Product *>::iterator end();
};

// Impl class methods implementations

ProductContainer::Impl::Impl()
{
  pricingStrategy = nullptr;
}

ProductContainer::Impl::~Impl()
{
  for (Product *p : data)
  {
    delete p;
  }
}

ProductContainer::Impl *ProductContainer::Impl::clone() const
{
  Impl *newImpl = new Impl();
  newImpl->setPricingStrategy(pricingStrategy);
  for (Product *p : data)
    newImpl->push_back(p->clone());
  return newImpl;
}

void ProductContainer::Impl::forEach(std::function<void(Product *)> callback)
{
  std::for_each(data.begin(), data.end(), callback);
}

void ProductContainer::Impl::setPricingStrategy(PricingStrategy *pricingStrategy)
{
  this->pricingStrategy = pricingStrategy;
}

double ProductContainer::Impl::recalculatePrice(ForwardIterator &position) const
{
  Product *p = get(position);
  if (p == nullptr || pricingStrategy == nullptr)
    throw StrategyNotSetException("PricingStrategy");

  return pricingStrategy->calculatePrice(p->calculatePrice());
}

void ProductContainer::Impl::push_back(Product *p)
{
  data.push_back(p);
}

void ProductContainer::Impl::insert(ForwardIterator &position, Product *p)
{
  // this and others below would break if ProductContainer wasn't a friend of ForwardIterator
  data.insert(position.pIterImpl->getIt(), p); // inserts before the iterator position
}

Product *ProductContainer::Impl::get(const ForwardIterator &position) const
{
  return position.pIterImpl->dereference();
}

void ProductContainer::Impl::update(const ForwardIterator &position, Product *p)
{
  delete *position.pIterImpl->getIt(); // free old product
  *position.pIterImpl->getIt() = p;    // replace with new
}

void ProductContainer::Impl::remove(std::vector<Product *>::iterator position)
{
  delete *position;
  data.erase(position);
}

std::vector<Product *>::iterator ProductContainer::Impl::begin()
{
  return data.begin();
}

std::vector<Product *>::iterator ProductContainer::Impl::end()
{
  return data.end();
}

// -------------------------------------------------
class ProductContainer::ForwardIterator::IteratorImpl
{
private:
  std::vector<Product *>::iterator it;

public:
  IteratorImpl(std::vector<Product *>::iterator i);
  IteratorImpl(const IteratorImpl &other);

  std::vector<Product *>::iterator getIt() const;

  Product *&dereference();
  void increment();
  bool equals(const IteratorImpl &other) const;
};

// IteratorImpl class methods implementations

ProductContainer::ForwardIterator::IteratorImpl::IteratorImpl(std::vector<Product *>::iterator i)
{
  it = i;
}

ProductContainer::ForwardIterator::IteratorImpl::IteratorImpl(const ProductContainer::ForwardIterator::IteratorImpl &other)
{
  it = other.it;
}

std::vector<Product *>::iterator ProductContainer::ForwardIterator::IteratorImpl::getIt() const
{
  return it;
}

Product *&ProductContainer::ForwardIterator::IteratorImpl::dereference()
{
  return *it;
}

void ProductContainer::ForwardIterator::IteratorImpl::increment()
{
  ++it;
}

bool ProductContainer::ForwardIterator::IteratorImpl::equals(const IteratorImpl &other) const
{
  return it == other.it;
}

// ForwardIterator class methods implementations

ProductContainer::ForwardIterator::ForwardIterator(IteratorImpl *impl)
{
  pIterImpl = impl;
}

ProductContainer::ForwardIterator::ForwardIterator(const ForwardIterator &other)
{
  pIterImpl = new IteratorImpl(*other.pIterImpl);
}

ProductContainer::ForwardIterator::~ForwardIterator()
{
  delete pIterImpl;
}

ProductContainer::ForwardIterator &ProductContainer::ForwardIterator::operator=(const ForwardIterator &other)
{
  if (this != &other)
  {
    IteratorImpl *newImpl = new IteratorImpl(*other.pIterImpl); // allocate first
    delete pIterImpl;                                           // only delete old if allocation succeeded
    pIterImpl = newImpl;
  }
  return *this;
}

Product *&ProductContainer::ForwardIterator::operator*()
{
  return pIterImpl->dereference();
}

ProductContainer::ForwardIterator &ProductContainer::ForwardIterator::operator++()
{
  pIterImpl->increment();
  return *this;
}

bool ProductContainer::ForwardIterator::operator!=(const ForwardIterator &other) const
{
  return !pIterImpl->equals(*other.pIterImpl); // I didn't know 'other' can access private fields here
}

bool ProductContainer::ForwardIterator::operator==(const ForwardIterator &other) const
{
  return pIterImpl->equals(*other.pIterImpl);
}

// ProductContainer class methods implementations

ProductContainer::ProductContainer()
{
  pImpl = new Impl();
}

ProductContainer::ProductContainer(const ProductContainer &other)
{
  pImpl = other.pImpl->clone();
}

ProductContainer::~ProductContainer()
{
  delete pImpl;
}

ProductContainer &ProductContainer::operator=(const ProductContainer &other)
{
  if (this != &other)
  {
    Impl *newImpl = other.pImpl->clone(); // allocate first
    delete pImpl;                         // only delete old if allocation succeeded
    pImpl = newImpl;
  }
  return *this;
}

void ProductContainer::forEach(std::function<void(Product *)> callback)
{
  pImpl->forEach(callback);
}

void ProductContainer::setPricingStrategy(PricingStrategy *s)
{
  pImpl->setPricingStrategy(s);
}

double ProductContainer::recalculatePrice(ForwardIterator &position) const
{
  return pImpl->recalculatePrice(position);
}

void ProductContainer::push_back(Product *p)
{
  pImpl->push_back(p);
}

void ProductContainer::insert(ForwardIterator &position, Product *product)
{
  pImpl->insert(position, product);
}

Product *ProductContainer::get(const ForwardIterator &position) const
{
  return pImpl->get(position);
}

void ProductContainer::update(const ForwardIterator &position, Product *p)
{
  pImpl->update(position, p);
}

void ProductContainer::remove(std::vector<Product *>::iterator position)
{
  pImpl->remove(position);
}

ProductContainer::ForwardIterator ProductContainer::begin()
{
  // this would break if ProductContainer wasn't a friend of ForwardIterator
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->begin()));
}

ProductContainer::ForwardIterator ProductContainer::end()
{
  // this would break if ProductContainer wasn't a friend of ForwardIterator
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->end()));
}

void ProductContainer::toString() const
{
  pImpl->forEach([](Product *p) {
    std::cout << p->toString() << std::endl;
  });
}
