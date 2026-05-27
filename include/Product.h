#ifndef PRODUCT_9614014
#define PRODUCT_9614014

#include <string>
#include <iostream>
#include <stdexcept>
#define EPSILON 1e-6

class Product
{
private: 
  std::string origin;
  double priceCoefficient;
  double transportationCostCoefficient;

protected:
  Product(const std::string &origin, double priceCoefficient, double transportationCostCoefficient);
public:
  virtual ~Product() = default;
  virtual Product *clone() const = 0;

  void setOrigin(const std::string &origin);
  void setPriceCoefficient(double priceCoefficient);
  void setTransportationCostCoefficient(double transportationCostCoefficient);

  std::string getOrigin() const;
  double getPriceCoefficient() const;
  double getTransportationCostCoefficient() const;

  virtual double calculatePrice() const = 0;
  virtual double calculateTransportationCost() const = 0;
  virtual double calculateProfit() const; // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
  
  // Compares by profit
  virtual bool operator==(const Product &other) const;
  virtual bool operator!=(const Product &other) const;
  virtual bool operator>(const Product &other) const;
  virtual bool operator<(const Product &other) const;
  virtual bool operator>=(const Product &other) const;
  virtual bool operator<=(const Product &other) const;

  // How much percentage of the product is repurposed (e.g. used for animal feed instead of human consumption)
  // Not all products need to implement this method
  virtual void repurposePercentage(double percentage); 

  virtual std::string toString() const;
};

#endif // PRODUCT_9614014