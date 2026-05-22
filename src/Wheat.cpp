#include <sstream>
#include "../include/Wheat.h"

Wheat::Wheat(const std::string &origin, double weight, double priceCoefficient, double transportationCostCoefficient) : Product(origin, priceCoefficient, transportationCostCoefficient)
{
  setWeight(weight);
}

void Wheat::setWeight(double weight)
{
  if (weight < 0)
    throw std::invalid_argument("Weight cannot be negative");
  this->weight = weight;
}

double Wheat::getWeight() const
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

void Wheat::repurposePercentage(double percentage)
{
  if (percentage < 0 || percentage > 100)
    throw std::invalid_argument("Percentage must be between 0 and 100");
  weight *= (1.0 - percentage / 100.0);
}

Wheat *Wheat::clone() const
{
  return new Wheat(*this);
}

 std::string Wheat::toString() const
 {
  std::stringstream ss;  
  ss << Product::toString();
  ss << "Type: Wheat" << std::endl;
  ss << "Weight: " << getWeight() << std::endl << std::endl;
  return ss.str();
 }