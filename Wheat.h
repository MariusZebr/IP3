#ifndef WHEAT_2896501
#define WHEAT_2896501

#include "Product.h"

class Wheat : public Product
{
private:
  double weight;

public:
  Wheat(const std::string &origin, double weight, double priceCoefficient, double transportationCostCoefficient);

  void setWeight(double weight);

  int getWeight() const;

  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  std::string toString() const override;
};

#endif // WHEAT_2896501