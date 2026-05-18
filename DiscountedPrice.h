#ifndef DISCOUNTEDPRICE_H_47879129
#define DISCOUNTEDPRICE_H_47879129

#include "PricingStrategy.h"

class DiscountedPrice : public PricingStrategy
{
private:
  double discountPercent;

public:
  DiscountedPrice(double discountPercent);
  double calculatePrice(double basePrice) override;
};

#endif // DISCOUNTEDPRICE_H_47879129