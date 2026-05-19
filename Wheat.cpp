#include <sstream>
#include "Product.h"

Wheat::Wheat(const std::string &origin, double weight, double priceCoefficient, double transportationCostCoefficient) : Product(origin, priceCoefficient, transportationCostCoefficient)
{
  setWeight(weight);
}

void Wheat::setWeight(double weight)
{
  this->weight = weight;
}

int Wheat::getWeight() const
{
  return weight;
}

double Wheat::calculatePrice() const
{
  return getWeight() * getPriceCoefficient();
}

double Wheat::calculateTransportationCost() const
{
  return getWeight() * getTransportationCostCoefficient();
}

void Wheat::repurpose(double percentage)
{
  weight -= weight * percentage / 100.0;
}

Wheat *Wheat::clone() const
{
  return new Wheat(*this);
}

 std::string Wheat::toString() const
 {
  std::stringstream ss;  
  ss << Product::toString() << std::endl;
  ss << getWeight() << std::endl;
 }