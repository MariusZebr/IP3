#include "include/strategies/DiscountedPrice.h"

DiscountedPrice::DiscountedPrice(double discountPercent)
{
  this->discountPercent = discountPercent;
}

double DiscountedPrice::calculatePrice(double basePrice)
{
  return basePrice * (1.0 - discountPercent / 100.0);
}