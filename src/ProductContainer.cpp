#include <vector>
#include <algorithm>
#include <sstream>
#include "../include/Product.h"
#include "../include/strategies/PricingStrategy.h"
#include "../include/ProductContainer.h"
#include "../include/exceptions/StrategyNotSetException.h"

class ProductContainer::Impl
{
private:
  // fields
  std::vector<Product *> data;
  PricingStrategy *pricingStrategy; // container does not manage the memory of the strategy

public:
  Impl();
  ~Impl();
  Impl *clone() const;

  void forEach(std::function<void(Product *)> callback);

  void setPricingStrategy(PricingStrategy *s);
  PricingStrategy *getPricingStrategy() const;

  double recalculatePrice(ForwardIterator &position) const;

  // CRUD operations

  void push_back(Product *p);
  void insert(ForwardIterator position, Product *p);
  Product *get(const ForwardIterator position) const;
  void update(const ForwardIterator position, Product *p);
  void remove(ForwardIterator position);

  // Iterator methods
  std::vector<Product *>::iterator begin();
  std::vector<Product *>::iterator end();

  int getSize() const;
  void clear();
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

PricingStrategy *ProductContainer::Impl::getPricingStrategy() const
{
  return pricingStrategy;
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

std::vector<Product *>::iterator ProductContainer::Impl::begin()
{
  return data.begin();
}

std::vector<Product *>::iterator ProductContainer::Impl::end()
{
  return data.end();
}

int ProductContainer::Impl::getSize() const
{
  return data.size();
}

void ProductContainer::Impl::clear()
{
  for (Product *p : data)
  {
    delete p;
  }
  data.clear();
}

// -------------------------------------------------
class ProductContainer::ForwardIterator::IteratorImpl
{
private:
  std::vector<Product *>::iterator it;
  std::vector<Product *>::iterator endIt;

public:
  IteratorImpl(std::vector<Product *>::iterator i, std::vector<Product *>::iterator end);
  IteratorImpl(const IteratorImpl &other);

  std::vector<Product *>::iterator getIt() const;
  std::vector<Product *>::iterator getItEnd() const;

  Product *&dereference();
  void increment();
  bool equals(const IteratorImpl &other) const;
};

void ProductContainer::Impl::insert(ForwardIterator position, Product *p)
{
  // this and others below would break if ProductContainer wasn't a friend of ForwardIterator
  data.insert(position.pIterImpl->getIt(), p); // inserts before the iterator position 
}

// Impl class methods that depend on the pIterImpl class
Product *ProductContainer::Impl::get(const ForwardIterator position) const
{
  return position.pIterImpl->dereference();
}

void ProductContainer::Impl::update(const ForwardIterator position, Product *p)
{
  if (position.pIterImpl->getIt() == position.pIterImpl->getItEnd())
    throw std::out_of_range("ProductContainer::update: cannot update end iterator");
  auto &slot = *position.pIterImpl->getIt(); // Product*& (reference to pointer)

  delete slot; // delete old object
  slot = p;    // replace pointer
}

void ProductContainer::Impl::remove(ForwardIterator position)
{
  if (position.pIterImpl->getIt() == position.pIterImpl->getItEnd())
    throw std::out_of_range("ProductContainer::remove: cannot remove end iterator");
  auto it = position.pIterImpl->getIt();
  delete *it;
  data.erase(it);
}

// IteratorImpl class methods implementations

ProductContainer::ForwardIterator::IteratorImpl::IteratorImpl(std::vector<Product *>::iterator i, std::vector<Product *>::iterator end)
{
  it = i;
  endIt = end;
}

ProductContainer::ForwardIterator::IteratorImpl::IteratorImpl(const ProductContainer::ForwardIterator::IteratorImpl &other)
{
  it = other.it;
  endIt = other.endIt;
}

std::vector<Product *>::iterator ProductContainer::ForwardIterator::IteratorImpl::getIt() const
{
  return it;
}

std::vector<Product *>::iterator ProductContainer::ForwardIterator::IteratorImpl::getItEnd() const
{
  return endIt;
}

Product *&ProductContainer::ForwardIterator::IteratorImpl::dereference()
{
  if (it == endIt)
    throw std::out_of_range("ForwardIterator: cannot dereference end iterator");
  return *it;
}

void ProductContainer::ForwardIterator::IteratorImpl::increment()
{
  if (it == endIt)
    throw std::out_of_range("ForwardIterator: cannot advance past end");
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

ProductContainer::ForwardIterator ProductContainer::ForwardIterator::operator++(int)
{
  ForwardIterator temp = *this;
  pIterImpl->increment();
  return temp;
}

bool ProductContainer::ForwardIterator::operator!=(const ForwardIterator &other) const
{
  return !pIterImpl->equals(*other.pIterImpl); // I didn't know 'other' can access its private fields here
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

std::string ProductContainer::listProducts() const
{
  std::stringstream ss;
  if (pImpl->getPricingStrategy() == nullptr)
    throw StrategyNotSetException("PricingStrategy");
  for (auto i = begin(); i != end(); ++i)
  {
    Product *p = *i;
    ss << p->toString();
    ss << "Price of Product with pricing strategy: " << recalculatePrice(i) << std::endl;
    ss << "Transportation Cost of Product: " << p->calculateTransportationCost() << std::endl;
    ss << "Profit of Product: " << p->calculateProfit() << std::endl;
    ss << std::endl;
  }
  return ss.str();
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

void ProductContainer::insert(ForwardIterator position, Product *product)
{
  pImpl->insert(position, product);
}

Product *ProductContainer::get(const ForwardIterator position) const
{
  return pImpl->get(position);
}

void ProductContainer::update(const ForwardIterator position, Product *p)
{
  pImpl->update(position, p);
}

void ProductContainer::remove(ForwardIterator position)
{
  pImpl->remove(position);
}

ProductContainer::ForwardIterator ProductContainer::begin() const
{
  // this would break if ProductContainer wasn't a friend of ForwardIterator
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->begin(), pImpl->end()));
}

ProductContainer::ForwardIterator ProductContainer::end() const
{
  // this would break if ProductContainer wasn't a friend of ForwardIterator
  return ForwardIterator(new ForwardIterator::IteratorImpl(pImpl->end(), pImpl->end()));
}

int ProductContainer::getSize() const
{
  return pImpl->getSize();
}

void ProductContainer::clear()
{
  pImpl->clear();
}

std::string ProductContainer::toString() const
{
  std::stringstream ss;
  pImpl->forEach([&ss](Product *p)
                 { ss << p->toString(); });
  return ss.str();
}
