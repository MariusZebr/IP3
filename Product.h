#ifndef PRODUCT_9614014
#define PRODUCT_9614014

#include <string>
#include <iostream>

class Product
{
private: 
  std::string origin;
  double priceCoefficient;
  double transportationCostCoefficient;

public:
  Product(const std::string &origin, double priceCoefficient, double transportationCostCoefficient);
  virtual ~Product();
  virtual Product *clone() const = 0;

  void setOrigin(const std::string &origin);
  void setPriceCoefficient(double priceCoefficient);
  void setTransportationCostCoefficient(double transportationCostCoefficient);

  std::string getOrigin() const;
  double getPriceCoefficient() const;
  double getTransportationCostCoefficient() const;

  virtual double calculatePrice() const = 0;
  virtual double calculateTransportationCost() const = 0;
  
  virtual void repurpose(double percentage); 

  virtual std::string toString() const;
};

class Wheat : public Product
{
private:
  double weight;

public:
  Wheat(const std::string &origin, double weight, double priceCoefficient, double transportationCostCoefficient);
  Wheat *clone() const override;

  void setWeight(double weight);

  int getWeight() const;

  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  void repurpose(double percentage) override;
  
  std::string toString() const override;
};

class Milk : public Product
{
private:
  double volume;
  bool isPasteurized;
  double pasterurizedCoefficient;

public:
  Milk(std::string origin, int priceCoefficient, 
    int transportationCostCoefficient, int volume, bool isPasteurized, double pasterurizedCoefficient);
  Milk *clone() const override;

  void setVolume(double volume);
  void setIsPasteurized(bool isPasteurized);
  void setPasterurizeCoefficient(double pasterurizedCoefficient);

  double getVolume() const;
  bool getIsPasteurized() const;
  double getPasterurizedPriceCoefficient() const;
  
  double calculatePrice() const override;
  double calculateTransportationCost() const override;

  void pasteurize();
  
  std::string toString() const override; 
};

#endif // PRODUCT_9614014