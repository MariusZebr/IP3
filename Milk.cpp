#include <sstream>
#include "Product.h"

Milk::Milk(std::string origin, int priceCoefficient,
           int transportationCostCoefficient, int volume, bool isPasteurized, double pasterurizedCoefficient) : Product(origin, priceCoefficient, transportationCostCoefficient)
{
  setVolume(volume);
  setIsPasteurized(isPasteurized);
  setPasterurizeCoefficient(pasterurizedCoefficient);
}

void Milk::setVolume(double volume)
{
  this->volume = volume;
}

void Milk::setIsPasteurized(bool isPasteurized)
{
  this->isPasteurized = isPasteurized;
}

void Milk::setPasterurizeCoefficient(double pasterurizeCoefficient)
{
  this->pasterurizedCoefficient = pasterurizeCoefficient;
}

double Milk::getVolume() const
{
  return volume;
}

bool Milk::getIsPasteurized() const
{
  return isPasteurized;
}

double Milk::getPasterurizedPriceCoefficient() const
{
  return pasterurizedCoefficient;
}

double Milk::calculatePrice() const
{
  double finalPrice;
  if (getIsPasteurized())
  {
    finalPrice = getVolume() * getPriceCoefficient();
  }
  else
  {
    finalPrice = getVolume() * getPasterurizedPriceCoefficient();
  }
  return finalPrice;
}

double Milk::calculateTransportationCost() const
{
  return getVolume() * getTransportationCostCoefficient();
}

void Milk::pasteurize()
{
  isPasteurized = true;
}

Milk *Milk::clone() const
{
  return new Milk(*this);
}

 std::string Milk::toString() const
 {
  std::stringstream ss;  
  ss << Product::toString() << std::endl;
  ss << getVolume() << std::endl;
  ss << getIsPasteurized() << std::endl;
  ss << getPasterurizedPriceCoefficient() << std::endl;
  return ss.str();
 }