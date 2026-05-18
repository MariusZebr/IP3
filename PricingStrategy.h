#ifndef PRICINGSTRATEGY_H_716230
#define PRICINGSTRATEGY_H_716230

class PricingStrategy
{
public:
  virtual double calculatePrice(double basePrice) = 0;
  virtual ~PricingStrategy() = default;
};

#endif // PRICINGSTRATEGY_H_716230