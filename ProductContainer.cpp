#include "Product.h"
#include "ProductContainer.h"
#include "PricingStrategy.h"
#include <vector>

class ProductContainer::Impl
{
private:
  // fields
  std::vector<Product *> data;
  PricingStrategy *pricingStrategy;

public:
  Impl();
  ~Impl();

  void setPricingStrategy(PricingStrategy *s);
  double recalculatePrice(int index) const;

  // CRUD operations

  void add(Product *p);
  Product *get(int index) const;
  void update(int index, Product *p);
  void remove(int index);

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

void ProductContainer::Impl::setPricingStrategy(PricingStrategy *pricingStrategy)
{
  this->pricingStrategy = pricingStrategy;
}

double ProductContainer::Impl::recalculatePrice(int index) const
  {
    Product *p = get(index);
    if (p == nullptr || pricingStrategy == nullptr)
      //

    return pricingStrategy->calculatePrice(p->calculatePrice());
  }

void ProductContainer::Impl::add(Product *p)
{
  data.push_back(p);
}

Product *ProductContainer::Impl::get(int index) const
{
  // exception?
  if (index >= data.size())
    return nullptr;

  return data[index];
}

void ProductContainer::Impl::update(int index, Product *p)
{
  // exception?
  if (index < data.size())
  {
    delete data[index];
    data[index] = p;
  }
}

void ProductContainer::Impl::remove(int index)
{
  // exception?
  if (index < data.size())
  {
    delete data[index];
    data.erase(data.begin() + index);
  }
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

ProductContainer::~ProductContainer()
{
  delete pImpl;
}

void ProductContainer::setPricingStrategy(PricingStrategy *s)
{
  pImpl->setPricingStrategy(s);
}

double ProductContainer::recalculatePrice(int index) const
{
  return pImpl->recalculatePrice(index);
}

ProductContainer::ForwardIterator ProductContainer::begin()
{
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->begin()));
}

ProductContainer::ForwardIterator ProductContainer::end()
{
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->end()));
}
