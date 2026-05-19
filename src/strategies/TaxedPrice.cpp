#include "include/strategies/TaxedPrice.h"

TaxedPrice::TaxedPrice(double percent)
{
  taxPercent = percent;
}

double TaxedPrice::calculatePrice(double basePrice)
{
  return basePrice * (1.0 + taxPercent / 100.0);
}