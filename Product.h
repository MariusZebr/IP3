#ifndef PRODUCT_9614014
#define PRODUCT_9614014

#include <string>
#include <iostream>

class Product
{
protected: // ?
  std::string origin;
  double priceCoefficient;
  double transportationCostCoefficient;

public:
  Product(const std::string &origin, double priceCoefficient, double transportationCostCoefficient);
  virtual ~Product(); //= default; // ?

  void setOrigin(const std::string &origin);
  void setPriceCoefficient(double priceCoefficient);
  void setTransportationCostCoefficient(double transportationCostCoefficient);

  std::string getOrigin() const;
  double getPriceCoefficient() const;
  double getTransportationCostCoefficient() const;

  virtual double calculatePrice() const = 0;
  virtual double calculateTransportationCost() const = 0;

  virtual std::string toString() const;
};

#endif // PRODUCT_9614014