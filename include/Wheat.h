#ifndef WHEAT_H_0925956
#define WHEAT_H_0925956

#include "Product.h"

class Wheat : public Product
{
private:
  double weight;

public:
  Wheat(const std::string &origin, double weight, double priceCoefficient, double transportationCostCoefficient); 
protected:
  Wheat(const Wheat& other);
public:
  Wheat *clone() const override;

  void setWeight(double weight);
  double getWeight() const;

  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  void repurposePercentage(double percentage) override;
  
  std::string toString() const override;
};

#endif // WHEAT_H_0925956