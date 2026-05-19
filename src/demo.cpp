#include <algorithm>
#include <iostream>
#include "../include/Product.h"
#include "../include/Milk.h"
#include "../include/Wheat.h"
#include "../include/ProductContainer.h"
#include "../include/strategies/DiscountedPrice.h"
#include "../include/exceptions/NotImplementedException.h"
#include "../include/exceptions/StrategyNotSetException.h"

int main()
{
  // std::string origin, double volume, double priceCoefficient,
  // double transportationCostCoefficient,
  // bool isPasteurized, double pasterurizedCoefficient
  Product *p1 = new Milk("Farm A", 100, 2.5, 0.5, false, 3.0);
  // const std::string &origin, double weight,
  // double priceCoefficient,
  // double transportationCostCoefficient
  Product *p2 = new Wheat("Farm B", 200, 1.0, 0.2);

  // polymorphism (calculatePrice and calculateTransportationCost)
  std::cout << "Price of milk from " << p1->getOrigin() << " is " << p1->calculatePrice() << std::endl;
  std::cout << "Price of wheat from " << p2->getOrigin() << " is " << p2->calculatePrice() << std::endl
            << std::endl;

  std::cout << "Transportation cost of milk from " << p1->getOrigin() << " is " << p1->calculateTransportationCost() << std::endl;
  std::cout << "Transportation cost of wheat from " << p2->getOrigin() << " is " << p2->calculateTransportationCost() << std::endl
            << std::endl;

  // should throw NotImplementedException since repurpose is not implemented for Milk
  try
  {
    // 10% repurposed
    p1->repurpose(10);
  }
  catch (const NotImplementedException &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  // repurpose is implemented for Wheat, so it should work without exceptions
  try
  {
    // 20% repurposed
    p2->repurpose(20);
    // accessing method of child class through base class pointer using dynamic_cast
    Wheat *wheat = dynamic_cast<Wheat *>(p2);
    if (wheat)
    {
      std::cout << "After repurposing, weight of wheat from " << wheat->getOrigin() << " is " << wheat->getWeight() << std::endl
                << std::endl;
    }
    else
    {
      std::cout << "dynamic_cast failed" << std::endl;
    }
  }
  catch (const std::exception &e)
  {
    std::cout << e.what() << '\n';
  }

  std::cout << "p1 profit (" << p1->calculateProfit() << ") == p2 profit (" << p2->calculateProfit() << "): " << (*p1 == *p2) << std::endl;
  std::cout << "p1 profit (" << p1->calculateProfit() << ") > p2 profit (" << p2->calculateProfit() << "): " << (*p1 > *p2) << std::endl;
  std::cout << "p1 profit (" << p1->calculateProfit() << ") < p2 profit (" << p2->calculateProfit() << "): " << (*p1 < *p2) << std::endl;
  std::cout << "p1 profit (" << p1->calculateProfit() << ") >= p2 profit (" << p2->calculateProfit() << "): " << (*p1 >= *p2) << std::endl;
  std::cout << "p1 profit (" << p1->calculateProfit() << ") <= p2 profit (" << p2->calculateProfit() << "): " << (*p1 <= *p2) << std::endl;
  std::cout << std::endl;

  ProductContainer container1;
  container1.push_back(p1);
  container1.push_back(p2);
  std::cout << "Container contents:" << std::endl;
  std::cout << container1.toString() << std::endl;

  // iterator
  // forward iterator
  auto it = std::find_if(container1.begin(), container1.end(), [](Product *p)
                         { return p->getOrigin() == "Farm A"; });

  ProductContainer container2;
  // deep copy
  container2 = container1;
  std::cout << "Container 2 contents after copy assignment:" << std::endl;
  std::cout << container2.toString() << std::endl;

  // strategy
  // 10% discount
  DiscountedPrice *discount = new DiscountedPrice(10); // container does not manage the memory of the strategy
  container1.setPricingStrategy(discount);
  std::cout << "Container 1 contents after setting discount strategy:" << std::endl;
  // should not throw since the strategy is set
  try
  {
    // goes through each product in the container and
    // calls Product class virtual methods calculatePrice, calculateTransportationCost and calculateProfit
    std::cout << container1.listProducts();
  }
  catch (const StrategyNotSetException &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  // should throw since the strategy is not set for container2
  std::cout << "Container 2 contents after not setting strategy:" << std::endl;
  try
  {
    std::cout << container2.listProducts();
  }
  catch (const StrategyNotSetException &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Iterating through container and calling callback:" << std::endl;
  // callback
  container1.forEach([](Product *p)
                     {
    std::cout << p->toString();
    std::cout << "Price of Product without pricing strategy: " << p->calculatePrice() << std::endl;
    std::cout << "Transportation Cost of Product: " << p->calculateTransportationCost() << std::endl;
    std::cout << "Profit of Product: " << p->calculateProfit() << std::endl << std::endl;});
  

  delete discount;
  delete p1;
  delete p2;
  return 0;
}