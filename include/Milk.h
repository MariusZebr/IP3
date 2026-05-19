#ifndef MILK_H_205700525
#define MILK_H_205700525

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
  Milk *clone() const override;

  void setVolume(double volume);
  void setIsPasteurized(bool isPasteurized);
  void setPasterurizeCoefficient(double pasterurizedCoefficient);

  double getVolume() const;
  bool getIsPasteurized() const;
  double getPasterurizedPriceCoefficient() const;
  
  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  void pasteurize();
  
  std::string toString() const override; 
};

#endif // MILK_H_205700525