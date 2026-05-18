#include <sstream>
#include "Product.h"

Product::Product(const std::string &origin, double priceCoefficient, double transportationCostCoefficient)
 {
    setOrigin(origin);
    setPriceCoefficient(priceCoefficient);
    setTransportationCostCoefficient(transportationCostCoefficient);
  }

  void Product::setOrigin(const std::string &origin)
  {
    this->origin = origin;
  }

  void Product::setPriceCoefficient(double priceCoefficient)
  {
    this->priceCoefficient = priceCoefficient;
  }

  void Product::setTransportationCostCoefficient(double transportationCostCoefficient) 
  {
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

  std::string Product::toString() const
  {
    std::stringstream ss;
    ss << "origin: "<< getOrigin() << std::endl;
    ss << "price coefficient: " << getPriceCoefficient() << std::endl;
    ss << "transportation cost coefficient: " << getTransportationCostCoefficient() << std::endl;
    return ss.str();
  }


