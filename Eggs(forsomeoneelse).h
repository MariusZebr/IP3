#ifndef EGGS_2324467
#define EGGS_2324467

#include <string>
#include "Product.h"

class Eggs : public Product
{
private:
  int units;

public:
  Eggs(std::string origin, int units) : Product(origin), units(units)
  {
  }

  double calculatePrice() const override
  {
    return units * 2;
  }

  double calculateTransportationCost() const override
  {
    return units / 10;
  }
};

#endif // EGGS_2324467