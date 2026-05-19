#ifndef PRICINGSTRATEGY_H_23546357
#define PRICINGSTRATEGY_H_23546357

#include "PricingStrategy.h"

class RegularPrice : public PricingStrategy
{
public:
  double calculatePrice(double basePrice) override;
};

#endif // PRICINGSTRATEGY_H_23546357