#ifndef MILK_073517
#define MILK_073517

#include <string>
#include "Product.h"

class Milk : public Product
{
private:
  double volume;
  bool isPasteurized;
  double pasterurizedCoefficient;

public:
  Milk(std::string origin, int priceCoefficient, 
    int transportationCostCoefficient, int volume, bool isPasteurized, double pasterurizedCoefficient);

  void setVolume(double volume);
  void setIsPasteurized(bool isPasteurized);
  void setPasterurizeCoefficient(double pasterurizedCoefficient);

  double getVolume() const;
  bool getIsPasteurized() const;
  double getPasterurizedPriceCoefficient() const;
  
  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  void pasteurize();

  Milk *clone() const override;
  
  std::string toString() const override; 
};

#endif // MILK_073517