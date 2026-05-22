#include <sstream>
#include "../include/Milk.h"

Milk::Milk(std::string origin, double volume, double priceCoefficient,
           double transportationCostCoefficient, bool isPasteurized, double pasterurizedCoefficient)
            : Product(origin, priceCoefficient, transportationCostCoefficient)
{
  setVolume(volume);
  setIsPasteurized(isPasteurized);
  setPasterurizeCoefficient(pasterurizedCoefficient);
}

void Milk::setVolume(double volume)
{
  this->volume = volume;
  if (volume < 0)
    throw std::invalid_argument("Volume cannot be negative");
}

void Milk::setIsPasteurized(bool isPasteurized)
{
  this->isPasteurized = isPasteurized;
}

void Milk::setPasterurizeCoefficient(double pasterurizeCoefficient)
{
  if (pasterurizeCoefficient < 0)
    throw std::invalid_argument("Pasterurized price coefficient cannot be negative");
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
    finalPrice = getVolume() * getPasterurizedPriceCoefficient();
  }
  else
  {
    finalPrice = getVolume()  * getPriceCoefficient();
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
  ss << Product::toString();
  ss << "Type: Milk" << std::endl;
  ss << "Volume: " << getVolume() << std::endl;
  ss << "Is Pasteurized: " << getIsPasteurized() << std::endl;
  ss << "Pasteurized Price Coefficient: " 
  << getPasterurizedPriceCoefficient() << std::endl << std::endl;
  return ss.str();
 }