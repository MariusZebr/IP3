#include <sstream>
#include "../include/Product.h"
#include "../include/exceptions/NotImplementedException.h"

Product::Product(const std::string &origin, double priceCoefficient, double transportationCostCoefficient)
{
  setOrigin(origin);
  setPriceCoefficient(priceCoefficient);
  setTransportationCostCoefficient(transportationCostCoefficient);
}

Product::Product(const Product& other)
{
  origin = other.getOrigin();
  priceCoefficient = other.getPriceCoefficient();
  transportationCostCoefficient = other.getTransportationCostCoefficient();
}

void Product::setOrigin(const std::string &origin)
{
  this->origin = origin;
}

void Product::setPriceCoefficient(double priceCoefficient)
{
  if (priceCoefficient < 0)
    throw std::invalid_argument("Price coefficient cannot be negative");
  this->priceCoefficient = priceCoefficient;
}

void Product::setTransportationCostCoefficient(double transportationCostCoefficient)
{
  if (transportationCostCoefficient < 0)
    throw std::invalid_argument("Transportation cost coefficient cannot be negative");
  this->transportationCostCoefficient = transportationCostCoefficient;
}

std::string Product::getOrigin() const
{
  return origin;
}

double Product::getPriceCoefficient() const
{
  return priceCoefficient;
}

double Product::getTransportationCostCoefficient() const
{
  return transportationCostCoefficient;
}

double Product::calculateProfit() const
{
  return calculatePrice() - calculateTransportationCost();
}

bool Product::operator==(const Product &other) const
{
  return std::abs(calculateProfit() - other.calculateProfit()) < EPSILON;
}

bool Product::operator!=(const Product &other) const
{
  return !(*this == other);
}

bool Product::operator<(const Product &other) const
{
  return calculateProfit() < other.calculateProfit() - EPSILON;
}

bool Product::operator>(const Product &other) const
{
  return other < *this;
}

bool Product::operator>=(const Product &other) const
{
  return other <= *this;
}

bool Product::operator<=(const Product &other) const
{
  return (*this < other) || (*this == other);
}

void Product::repurposePercentage(double percentage)
{
  throw NotImplementedException("repurpose");
}

std::string Product::toString() const
{
  std::stringstream ss;
  ss << "origin: " << getOrigin() << std::endl;
  ss << "price coefficient: " << getPriceCoefficient() << std::endl;
  ss << "transportation cost coefficient: " << getTransportationCostCoefficient() << std::endl;
  return ss.str();
}
