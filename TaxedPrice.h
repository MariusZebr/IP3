#ifndef TAXEDPRICE_H_030194124
#define TAXEDPRICE_H_030194124

#include "PricingStrategy.h"

class TaxedPrice : public PricingStrategy
{
private:
  double taxPercent;

public:
  TaxedPrice(double percent);
  
  double calculatePrice(double basePrice) override;
};

#endif