#define TEST_MODE
#ifdef TEST_MODE

#include <fstream>
#include <cmath>
#include "../include/Product.h"
#include "../include/Milk.h"
#include "../include/Wheat.h"
//#include "../include/Eggs.h"
#include "../include/ProductContainer.h"
#include "../include/strategies/DiscountedPrice.h"
#include "../include/exceptions/NotImplementedException.h"
#include "../include/exceptions/StrategyNotSetException.h"

#define EPSILON 1e-6

static int passed = 0;
static int failed = 0;

static void report(std::ofstream &log, const std::string &name, bool ok)
{
  log << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
  if (ok)
    ++passed;
  else
    ++failed;
}

static bool nearlyEqual(double a, double b, double epsilon)
{
  return std::abs(a - b) < epsilon;
}

int main()
{
  std::ofstream fout;
  try
  {
    fout.open("log.txt");
    {
      Wheat *wheat1 = new Wheat("Farm A", 200, 1.0, 0.2);

      fout << "Testing Wheat class:" << std::endl;
      fout << "Testing getters:" << std::endl;
      report(fout, "Wheat getOrigin", wheat1->getOrigin() == "Farm A");
      report(fout, "Wheat getWeight", wheat1->getWeight() == 200);
      report(fout, "Wheat getPriceCoefficient", wheat1->getPriceCoefficient() == 1.0);
      report(fout, "Wheat getTransportationCostCoefficient", wheat1->getTransportationCostCoefficient() == 0.2);

      fout << "\nTesting setters:" << std::endl;
      wheat1->setOrigin("Farm B");
      report(fout, "Wheat setOrigin", wheat1->getOrigin() == "Farm B");

      wheat1->setPriceCoefficient(2.5);
      report(fout, "Wheat setPriceCoefficient", wheat1->getPriceCoefficient() == 2.5);
      try
      {
        wheat1->setPriceCoefficient(-1.0);
        report(fout, "Wheat setPriceCoefficient with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Wheat setPriceCoefficient with negative value", true);
      }

      wheat1->setTransportationCostCoefficient(0.5);
      report(fout, "Wheat setTransportationCostCoefficient", wheat1->getTransportationCostCoefficient() == 0.5);
      try
      {
        wheat1->setTransportationCostCoefficient(-0.5);
        report(fout, "Wheat setTransportationCostCoefficient with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Wheat setTransportationCostCoefficient with negative value", true);
      }

      wheat1->setWeight(250.9);
      report(fout, "Wheat setWeight", wheat1->getWeight() == 250.9);

      try
      {
        wheat1->setWeight(-10);
        report(fout, "Wheat setWeight with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Wheat setWeight with negative value", true);
      }
      delete wheat1;
    }

    fout << "\nTesting constructors:" << std::endl;

    try
    {
      Wheat *wheat1 = new Wheat("Farm A", -200, 1.0, 0.2);
      report(fout, "Wheat constructor with negative weight", false);
      delete wheat1;
    }
    catch (const std::invalid_argument &e)
    {
      report(fout, "Wheat constructor with negative weight", true);
    }

    try
    {
      Wheat *wheat1 = new Wheat("Farm A", 200, -1.0, 0.2);
      report(fout, "Wheat constructor with negative price coefficient", false);
      delete wheat1;
    }
    catch (const std::invalid_argument &e)
    {
      report(fout, "Wheat constructor with negative price coefficient", true);
    }

    try
    {
      Wheat *wheat1 = new Wheat("Farm A", 200, 1.0, -0.2);
      report(fout, "Wheat constructor with negative transportation cost coefficient", false);
      delete wheat1;
    }
    catch (const std::invalid_argument &e)
    {
      report(fout, "Wheat constructor with negative transportation cost coefficient", true);
    }

    {
      fout << std::endl
           << "Testing calculatePrice, calculateTransportationCost and calculateProfit:" << std::endl;
      Wheat *wheat1 = new Wheat("Farm C", 300, 1.5, 0.3);
      report(fout, "Wheat calculatePrice", nearlyEqual(wheat1->calculatePrice(), 450.0, EPSILON));
      report(fout, "Wheat calculateTransportationCost", nearlyEqual(wheat1->calculateTransportationCost(), 90.0, EPSILON));
      // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
      report(fout, "Wheat calculateProfit", nearlyEqual(wheat1->calculateProfit(), 360.0, EPSILON));

      fout << std::endl
           << "Testing repurposePercentage:" << std::endl;
      wheat1->repurposePercentage(10.5);
      report(fout, "Wheat repurposePercentage", nearlyEqual(wheat1->getWeight(), 268.5, EPSILON));
      try
      {
        wheat1->repurposePercentage(-5);
        report(fout, "Wheat repurposePercentage with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Wheat repurposePercentage with negative value", true);
      }

      try
      {
        wheat1->repurposePercentage(150);
        report(fout, "Wheat repurposePercentage with value greater than 100", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Wheat repurposePercentage with value greater than 100", true);
      }
      delete wheat1;
    }

    {
      fout << std::endl
           << "Testing comparison operators:" << std::endl;
      fout << "Testing when wheat2 is less than wheat3:" << std::endl;
      Wheat *wheat2 = new Wheat("Farm D", 400, 2.0, 0.4);
      Wheat *wheat3 = new Wheat("Farm E", 500, 2.5, 0.5);
      report(fout, "Wheat operator==", (*wheat2 == *wheat3) == false);
      report(fout, "Wheat operator>", (*wheat2 > *wheat3) == false);
      report(fout, "Wheat operator<", (*wheat2 < *wheat3) == true);
      report(fout, "Wheat operator>=", (*wheat2 >= *wheat3) == false);
      report(fout, "Wheat operator<=", (*wheat2 <= *wheat3) == true);
      delete wheat2;
      delete wheat3;
    }

    {
      fout << std::endl
           << "Testing when wheat2 is more than wheat3:" << std::endl;
      Wheat *wheat2 = new Wheat("Farm D", 500, 2.5, 0.5);
      Wheat *wheat3 = new Wheat("Farm E", 400, 2.0, 0.4);
      report(fout, "Wheat operator==", (*wheat2 == *wheat3) == false);
      report(fout, "Wheat operator>", (*wheat2 > *wheat3) == true);
      report(fout, "Wheat operator<", (*wheat2 < *wheat3) == false);
      report(fout, "Wheat operator>=", (*wheat2 >= *wheat3) == true);
      report(fout, "Wheat operator<=", (*wheat2 <= *wheat3) == false);
      delete wheat2;
      delete wheat3;
    }

    {
      fout << std::endl
           << "1. Testing when wheat2 is equal to wheat3:" << std::endl;
      Wheat *wheat2 = new Wheat("Farm D", 500, 2.5, 0.5);
      Wheat *wheat3 = new Wheat("Farm E", 500, 2.5, 0.5);
      report(fout, "Wheat operator==", (*wheat2 == *wheat3) == true);
      report(fout, "Wheat operator>", (*wheat2 > *wheat3) == false);
      report(fout, "Wheat operator<", (*wheat2 < *wheat3) == false);
      report(fout, "Wheat operator>=", (*wheat2 >= *wheat3) == true);
      report(fout, "Wheat operator<=", (*wheat2 <= *wheat3) == true);
      delete wheat2;
      delete wheat3;
    }

    {
      fout << std::endl
           << "2. Testing when wheat2 is equal to wheat3:" << std::endl;
      Wheat *wheat2 = new Wheat("Farm D", 500, 2.5, 0.5);
      Wheat *wheat3 = new Wheat("Farm E", 5000, 0.9, 0.7);
      report(fout, "Wheat operator==", (*wheat2 == *wheat3) == true);
      report(fout, "Wheat operator>", (*wheat2 > *wheat3) == false);
      report(fout, "Wheat operator<", (*wheat2 < *wheat3) == false);
      report(fout, "Wheat operator>=", (*wheat2 >= *wheat3) == true);
      report(fout, "Wheat operator<=", (*wheat2 <= *wheat3) == true);
      delete wheat2;
      delete wheat3;
    }

    {
      fout << std::endl
           << "Testing polymorphism:" << std::endl;
      Product *product1 = new Wheat("Farm A", 200, 2.5, 0.5);
      if (auto *w = dynamic_cast<Wheat *>(product1))
      {
        report(fout, "Wheat dynamic_cast and getWeight from Product class", nearlyEqual(w->getWeight(), 200.0, EPSILON));
      }
      else
      {
        report(fout, "Wheat dynamic_cast and getWeight from Product class", false);
      }
      Product *product2 = new Wheat("Farm A", 200, 2.5, 0.5);
      if (auto *w = dynamic_cast<Wheat *>(product2))
      {
        w->setWeight(100);
        report(fout, "Wheat dynamic_cast and setWeight", nearlyEqual(w->getWeight(), 100.0, EPSILON));
      }
      else
      {
        report(fout, "Wheat dynamic_cast and setWeight", false);
      }
      delete product2;
      delete product1;
    }

    {
      Product *product3 = new Wheat("Farm A", 100, 2.5, 0.5);
      report(fout, "Wheat calculatePrice through Product pointer", nearlyEqual(product3->calculatePrice(), 250.0, EPSILON));
      report(fout, "Wheat calculateTransportationCost through Product pointer", nearlyEqual(product3->calculateTransportationCost(), 50.0, EPSILON));
      // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
      report(fout, "Wheat calculateProfit through Product pointer", nearlyEqual(product3->calculateProfit(), 200.0, EPSILON));
      delete product3;
    }

    {
      fout << std::endl
           << "Testing deep copy:" << std::endl;
      Wheat *original = new Wheat("Farm A", 100, 2.5, 0.5);
      Wheat *copy = original->clone();
      report(fout, "Wheat deep copy (getWeight)", nearlyEqual(copy->getWeight(), original->getWeight(), EPSILON));
      report(fout, "Wheat deep copy (getPriceCoefficient)", nearlyEqual(copy->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
      report(fout, "Wheat deep copy (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
      delete original;
      delete copy;
    }

    {
      Wheat *original = new Wheat("Farm A", 100, 2.5, 0.5);
      Wheat *copy = original->clone();
      original->setWeight(200);
      report(fout, "Wheat deep copy independence (getWeight)", nearlyEqual(copy->getWeight(), 100.0, EPSILON));
      original->setPriceCoefficient(3.0);
      report(fout, "Wheat deep copy independence (getPriceCoefficient)", nearlyEqual(copy->getPriceCoefficient(), 2.5, EPSILON));
      original->setTransportationCostCoefficient(0.8);
      report(fout, "Wheat deep copy independence (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), 0.5, EPSILON));
      delete original;
      delete copy;
    }

    {
      fout << std::endl
           << "Testing clone method through Product pointer:" << std::endl;
      Product *original = new Wheat("Farm A", 100, 2.5, 0.5);
      Product *clone = original->clone();
      Wheat *clone_dynamic = dynamic_cast<Wheat *>(clone);
      Wheat *original_dynamic = dynamic_cast<Wheat *>(original);
      if (clone_dynamic && original_dynamic)
      {
        report(fout, "Wheat deep copy (getWeight with dynamic_cast)", nearlyEqual(clone_dynamic->getWeight(), original_dynamic->getWeight(), EPSILON));
      }
      else
      {
        report(fout, "Wheat deep copy (getWeight with dynamic_cast)", false);
      }

      report(fout, "Wheat deep copy (getPriceCoefficient)", nearlyEqual(clone->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
      report(fout, "Wheat deep copy (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
      delete original;
      delete clone;
    }

    {
      Product *original = new Wheat("Farm A", 100, 2.5, 0.5);
      Product *clone = original->clone();
      Wheat *clone_dynamic = dynamic_cast<Wheat *>(clone);
      Wheat *original_dynamic = dynamic_cast<Wheat *>(original);
      if (clone_dynamic && original_dynamic)
      {
        original_dynamic->setWeight(200);
        report(fout, "Wheat deep copy independence (getWeight with dynamic_cast)", nearlyEqual(clone_dynamic->getWeight(), 100.0, EPSILON));
      }
      else
      {
        report(fout, "Wheat deep copy independence (getWeight with dynamic_cast)", false);
      }
      original->setPriceCoefficient(3.0);
      report(fout, "Wheat deep copy independence (getPriceCoefficient)", nearlyEqual(clone->getPriceCoefficient(), 2.5, EPSILON));
      original->setTransportationCostCoefficient(0.8);
      report(fout, "Wheat deep copy independence (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), 0.5, EPSILON));
      delete original;
      delete clone;

      fout << std::endl
           << "Testing Milk class:" << std::endl;

      Milk *milk1 = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);

      fout << "Testing getters:" << std::endl;
      report(fout, "Milk getOrigin", milk1->getOrigin() == "Farm A");
      report(fout, "Milk getVolume", milk1->getVolume() == 100.0);
      report(fout, "Milk getPriceCoefficient", milk1->getPriceCoefficient() == 2.5);
      report(fout, "Milk getTransportationCostCoefficient", milk1->getTransportationCostCoefficient() == 0.5);
      report(fout, "Milk getIsPasteurized", milk1->getIsPasteurized() == false);
      report(fout, "Milk getPasterurizedPriceCoefficient", milk1->getPasterurizedPriceCoefficient() == 1.2);

      fout << std::endl
           << "Testing setters:" << std::endl;

      milk1->setOrigin("Farm B");
      report(fout, "Milk setOrigin", milk1->getOrigin() == "Farm B");

      milk1->setVolume(200.0);
      report(fout, "Milk setVolume", milk1->getVolume() == 200.0);
      try
      {
        milk1->setVolume(-10.0);
        report(fout, "Milk setVolume with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk setVolume with negative value", true);
      }

      milk1->setPriceCoefficient(3.0);
      report(fout, "Milk setPriceCoefficient", milk1->getPriceCoefficient() == 3.0);
      try
      {
        milk1->setPriceCoefficient(-1.0);
        report(fout, "Milk setPriceCoefficient with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk setPriceCoefficient with negative value", true);
      }

      milk1->setTransportationCostCoefficient(0.8);
      report(fout, "Milk setTransportationCostCoefficient", milk1->getTransportationCostCoefficient() == 0.8);
      try
      {
        milk1->setTransportationCostCoefficient(-0.5);
        report(fout, "Milk setTransportationCostCoefficient with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk setTransportationCostCoefficient with negative value", true);
      }

      milk1->setIsPasteurized(true);
      report(fout, "Milk setIsPasteurized", milk1->getIsPasteurized() == true);

      milk1->setPasterurizeCoefficient(1.5);
      report(fout, "Milk setPasterurizeCoefficient", milk1->getPasterurizedPriceCoefficient() == 1.5);
      try
      {
        milk1->setPasterurizeCoefficient(-0.5);
        report(fout, "Milk setPasterurizeCoefficient with negative value", false);
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk setPasterurizeCoefficient with negative value", true);
      }

      delete milk1;

      fout << std::endl
           << "Testing constructors:" << std::endl;

      try
      {
        Milk *milk1 = new Milk("Farm A", -100.0, 2.5, 0.5, false, 1.2);
        report(fout, "Milk constructor with negative volume", false);
        delete milk1;
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk constructor with negative volume", true);
      }

      try
      {
        Milk *milk1 = new Milk("Farm A", 100.0, -2.5, 0.5, false, 1.2);
        report(fout, "Milk constructor with negative price coefficient", false);
        delete milk1;
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk constructor with negative price coefficient", true);
      }

      try
      {
        Milk *milk1 = new Milk("Farm A", 100.0, 2.5, -0.5, false, 1.2);
        report(fout, "Milk constructor with negative transportation cost coefficient", false);
        delete milk1;
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk constructor with negative transportation cost coefficient", true);
      }

      try
      {
        Milk *milk1 = new Milk("Farm A", 100.0, 2.5, 0.5, false, -1.2);
        report(fout, "Milk constructor with negative pasteurized coefficient", false);
        delete milk1;
      }
      catch (const std::invalid_argument &e)
      {
        report(fout, "Milk constructor with negative pasteurized coefficient", true);
      }

      fout << std::endl
           << "Testing calculatePrice, calculateTransportationCost and calculateProfit:" << std::endl;

      // Not pasteurized: price = volume * priceCoeff = 200 * 2.5 = 500
      //                 transport = volume * transportCoeff = 200 * 0.4 = 80
      //                 profit = 500 - 80 = 420
      milk1 = new Milk("Farm C", 200.0, 2.5, 0.4, false, 1.2);
      report(fout, "Milk calculatePrice (not pasteurized)", nearlyEqual(milk1->calculatePrice(), 500.0, EPSILON));
      report(fout, "Milk calculateTransportationCost (not pasteurized)", nearlyEqual(milk1->calculateTransportationCost(), 80.0, EPSILON));
      // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
      report(fout, "Milk calculateProfit (not pasteurized)", nearlyEqual(milk1->calculateProfit(), 420.0, EPSILON));
      delete milk1;

      // Pasteurized: price = volume * pastCoeff = 200 * 5 = 1000
      //              transport = 200 * 0.4 = 80
      //              profit = 1000 - 80 = 920
      milk1 = new Milk("Farm C", 200.0, 2.5, 0.4, true, 5.0);
      report(fout, "Milk calculatePrice (pasteurized)", nearlyEqual(milk1->calculatePrice(), 1000.0, EPSILON));
      report(fout, "Milk calculateTransportationCost (pasteurized)", nearlyEqual(milk1->calculateTransportationCost(), 80.0, EPSILON));
      // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
      report(fout, "Milk calculateProfit (pasteurized)", nearlyEqual(milk1->calculateProfit(), 920.0, EPSILON));
      delete milk1;

      fout << std::endl
           << "Testing pasteurize:" << std::endl;

      milk1 = new Milk("Farm A", 100.0, 2.5, 0.5, false, 5.0);
      report(fout, "Milk getIsPasteurized before pasteurize", milk1->getIsPasteurized() == false);
      milk1->pasteurize();
      report(fout, "Milk getIsPasteurized after pasteurize", milk1->getIsPasteurized() == true);
      // Price should now use pasteurizedCoefficient
      report(fout, "Milk calculatePrice after pasteurize", nearlyEqual(milk1->calculatePrice(), 500.0, EPSILON));
      delete milk1;

      fout << std::endl
           << "Testing repurposePercentage:" << std::endl;

      // volume = 200, repurpose 10.5 % → 200 * (1 - 0.105) = 200 * 0.895 = 179.0
      milk1 = new Milk("Farm A", 200.0, 2.5, 0.5, false, 1.2);
      try
      {
        milk1->repurposePercentage(10.5);
        report(fout, "Milk repurposePercentage is not implemented", false);
      }
      catch (const NotImplementedException &e)
      {
        report(fout, "Milk repurposePercentage is not implemented", true);
      }

      delete milk1;

      // Operators compare by calculateProfit(). isPasteurized=false so pastCoeff is irrelevant here.
      fout << std::endl
           << "Testing comparison operators:" << std::endl;

      // milk2 profit = 400*(2.0-0.4) = 640   milk3 profit = 500*(2.5-0.5) = 1000
      fout << "Testing when milk2 is less than milk3:" << std::endl;
      Milk *milk2 = new Milk("Farm D", 400.0, 2.0, 0.4, false, 1.0);
      Milk *milk3 = new Milk("Farm E", 500.0, 2.5, 0.5, false, 1.0);
      report(fout, "Milk operator==", (*milk2 == *milk3) == false);
      report(fout, "Milk operator>", (*milk2 > *milk3) == false);
      report(fout, "Milk operator<", (*milk2 < *milk3) == true);
      report(fout, "Milk operator>=", (*milk2 >= *milk3) == false);
      report(fout, "Milk operator<=", (*milk2 <= *milk3) == true);
      delete milk2;
      delete milk3;

      // milk2 profit = 500*(2.5-0.5) = 1000   milk3 profit = 400*(2.0-0.4) = 640
      fout << std::endl
           << "Testing when milk2 is more than milk3:" << std::endl;
      milk2 = new Milk("Farm D", 500.0, 2.5, 0.5, false, 1.0);
      milk3 = new Milk("Farm E", 400.0, 2.0, 0.4, false, 1.0);
      report(fout, "Milk operator==", (*milk2 == *milk3) == false);
      report(fout, "Milk operator>", (*milk2 > *milk3) == true);
      report(fout, "Milk operator<", (*milk2 < *milk3) == false);
      report(fout, "Milk operator>=", (*milk2 >= *milk3) == true);
      report(fout, "Milk operator<=", (*milk2 <= *milk3) == false);
      delete milk2;
      delete milk3;

      // Both profit = 500*(2.5-0.5) = 1000
      fout << std::endl
           << "1. Testing when milk2 is equal to milk3:" << std::endl;
      milk2 = new Milk("Farm D", 500.0, 2.5, 0.5, false, 1.0);
      milk3 = new Milk("Farm E", 500.0, 2.5, 0.5, false, 1.0);
      report(fout, "Milk operator==", (*milk2 == *milk3) == true);
      report(fout, "Milk operator>", (*milk2 > *milk3) == false);
      report(fout, "Milk operator<", (*milk2 < *milk3) == false);
      report(fout, "Milk operator>=", (*milk2 >= *milk3) == true);
      report(fout, "Milk operator<=", (*milk2 <= *milk3) == true);
      delete milk2;
      delete milk3;

      // milk2: 500*(2.5-0.5)=1000   milk3: 1000*(1.5-0.5)=1000  — same profit, different params
      fout << std::endl
           << "2. Testing when milk2 is equal to milk3:" << std::endl;
      milk2 = new Milk("Farm D", 500.0, 2.5, 0.5, false, 1.0);
      milk3 = new Milk("Farm E", 1000.0, 1.5, 0.5, false, 1.0);
      report(fout, "Milk operator==", (*milk2 == *milk3) == true);
      report(fout, "Milk operator>", (*milk2 > *milk3) == false);
      report(fout, "Milk operator<", (*milk2 < *milk3) == false);
      report(fout, "Milk operator>=", (*milk2 >= *milk3) == true);
      report(fout, "Milk operator<=", (*milk2 <= *milk3) == true);
      delete milk2;
      delete milk3;

      fout << std::endl
           << "Testing polymorphism:" << std::endl;

      Product *product1 = new Milk("Farm A", 200.0, 2.5, 0.5, false, 1.2);
      if (auto *m = dynamic_cast<Milk *>(product1))
      {
        report(fout, "Milk dynamic_cast and getVolume from Product class", nearlyEqual(m->getVolume(), 200.0, EPSILON));
      }
      else
      {
        report(fout, "Milk dynamic_cast and getVolume from Product class", false);
      }

      Product *product2 = new Milk("Farm A", 200.0, 2.5, 0.5, false, 1.2);
      if (auto *m = dynamic_cast<Milk *>(product2))
      {
        m->setVolume(150.0);
        report(fout, "Milk dynamic_cast and setVolume", nearlyEqual(m->getVolume(), 150.0, EPSILON));
      }
      else
      {
        report(fout, "Milk dynamic_cast and setVolume", false);
      }

      // price = 100 * 2.5 = 250,  transport = 100 * 0.5 = 50,  profit = 200
      Product *product3 = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);
      report(fout, "Milk calculatePrice through Product pointer", nearlyEqual(product3->calculatePrice(), 250.0, EPSILON));
      report(fout, "Milk calculateTransportationCost through Product pointer", nearlyEqual(product3->calculateTransportationCost(), 50.0, EPSILON));
      // Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
      report(fout, "Milk calculateProfit through Product pointer", nearlyEqual(product3->calculateProfit(), 200.0, EPSILON));
      delete product3;
      delete product2;
      delete product1;

      fout << "\nTesting deep copy:" << std::endl;

      {
        Milk *original = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);
        Milk *copy = original->clone();
        report(fout, "Milk deep copy (getVolume)", nearlyEqual(copy->getVolume(), original->getVolume(), EPSILON));
        report(fout, "Milk deep copy (getPriceCoefficient)", nearlyEqual(copy->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
        report(fout, "Milk deep copy (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
        report(fout, "Milk deep copy (getIsPasteurized)", copy->getIsPasteurized() == original->getIsPasteurized());
        report(fout, "Milk deep copy (getPasterurizedPriceCoefficient)", nearlyEqual(copy->getPasterurizedPriceCoefficient(), original->getPasterurizedPriceCoefficient(), EPSILON));
        delete original;
        delete copy;
      }

      {
        Milk *original = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);
        Milk *copy = original->clone();
        original->setVolume(200.0);
        report(fout, "Milk deep copy independence (getVolume)", nearlyEqual(copy->getVolume(), 100.0, EPSILON));
        original->setPriceCoefficient(3.0);
        report(fout, "Milk deep copy independence (getPriceCoefficient)", nearlyEqual(copy->getPriceCoefficient(), 2.5, EPSILON));
        original->setTransportationCostCoefficient(0.8);
        report(fout, "Milk deep copy independence (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), 0.5, EPSILON));
        original->setIsPasteurized(true);
        report(fout, "Milk deep copy independence (getIsPasteurized)", copy->getIsPasteurized() == false);
        original->setPasterurizeCoefficient(2.0);
        report(fout, "Milk deep copy independence (getPasterurizedPriceCoefficient)", nearlyEqual(copy->getPasterurizedPriceCoefficient(), 1.2, EPSILON));
        delete original;
        delete copy;
      }

      fout << "Testing clone method through Product pointer:" << std::endl;

      {
        Product *original = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);
        Product *clone = original->clone();
        Milk *clone_dynamic = dynamic_cast<Milk *>(clone);
        Milk *orig_dynamic = dynamic_cast<Milk *>(original);
        if (clone_dynamic && orig_dynamic)
        {
          report(fout, "Milk deep copy (getVolume with dynamic_cast)", nearlyEqual(clone_dynamic->getVolume(), orig_dynamic->getVolume(), EPSILON));
        }
        else
        {
          report(fout, "Milk deep copy (getVolume with dynamic_cast)", false);
        }
        report(fout, "Milk deep copy (getPriceCoefficient)", nearlyEqual(clone->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
        report(fout, "Milk deep copy (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
        delete original;
        delete clone;
      }

      {
        Product *original = new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2);
        Product *clone = original->clone();
        Milk *clone_dynamic = dynamic_cast<Milk *>(clone);
        Milk *orig_dynamic = dynamic_cast<Milk *>(original);
        if (clone_dynamic && orig_dynamic)
        {
          orig_dynamic->setVolume(200.0);
          report(fout, "Milk deep copy independence (getVolume with dynamic_cast)", nearlyEqual(clone_dynamic->getVolume(), 100.0, EPSILON));
        }
        else
        {
          report(fout, "Milk deep copy independence (getVolume with dynamic_cast)", false);
        }
        original->setPriceCoefficient(3.0);
        report(fout, "Milk deep copy independence (getPriceCoefficient)", nearlyEqual(clone->getPriceCoefficient(), 2.5, EPSILON));
        original->setTransportationCostCoefficient(0.8);
        report(fout, "Milk deep copy independence (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), 0.5, EPSILON));
        delete original;
        delete clone;
      }
    }

    /*
    fout << "Testing Egg class:" << std::endl;

Egg *egg1 = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);

fout << "Testing getters:" << std::endl;
report(fout, "Egg getOrigin",                         egg1->getOrigin() == "Farm A");
report(fout, "Egg getCount",                          nearlyEqual(egg1->getCount(), 100.0, EPSILON));
report(fout, "Egg getPriceCoefficient",               nearlyEqual(egg1->getPriceCoefficient(), 2.5, EPSILON));
report(fout, "Egg getTransportationCostCoefficient",  nearlyEqual(egg1->getTransportationCostCoefficient(), 0.5, EPSILON));
report(fout, "Egg getIsOrganic",                      egg1->getIsOrganic() == false);
report(fout, "Egg getOrganicCoefficient",             nearlyEqual(egg1->getOrganicCoefficient(), 1.3, EPSILON));

fout << "\nTesting setters:" << std::endl;

egg1->setOrigin("Farm B");
report(fout, "Egg setOrigin", egg1->getOrigin() == "Farm B");

egg1->setCount(200.0);
report(fout, "Egg setCount", nearlyEqual(egg1->getCount(), 200.0, EPSILON));
try
{
  egg1->setCount(-10.0);
  report(fout, "Egg setCount with negative value", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg setCount with negative value", true);
}

egg1->setPriceCoefficient(3.0);
report(fout, "Egg setPriceCoefficient", nearlyEqual(egg1->getPriceCoefficient(), 3.0, EPSILON));
try
{
  egg1->setPriceCoefficient(-1.0);
  report(fout, "Egg setPriceCoefficient with negative value", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg setPriceCoefficient with negative value", true);
}

egg1->setTransportationCostCoefficient(0.8);
report(fout, "Egg setTransportationCostCoefficient", nearlyEqual(egg1->getTransportationCostCoefficient(), 0.8, EPSILON));
try
{
  egg1->setTransportationCostCoefficient(-0.5);
  report(fout, "Egg setTransportationCostCoefficient with negative value", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg setTransportationCostCoefficient with negative value", true);
}

egg1->setIsOrganic(true);
report(fout, "Egg setIsOrganic", egg1->getIsOrganic() == true);

egg1->setOrganicCoefficient(1.5);
report(fout, "Egg setOrganicCoefficient", nearlyEqual(egg1->getOrganicCoefficient(), 1.5, EPSILON));
try
{
  egg1->setOrganicCoefficient(-0.5);
  report(fout, "Egg setOrganicCoefficient with negative value", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg setOrganicCoefficient with negative value", true);
}

delete egg1;

fout << "\nTesting constructors:" << std::endl;

try
{
  Egg *egg1 = new Egg("Farm A", -100.0, 2.5, 0.5, false, 1.3);
  report(fout, "Egg constructor with negative count", false);
  delete egg1;
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg constructor with negative count", true);
}

try
{
  Egg *egg1 = new Egg("Farm A", 100.0, -2.5, 0.5, false, 1.3);
  report(fout, "Egg constructor with negative price coefficient", false);
  delete egg1;
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg constructor with negative price coefficient", true);
}

try
{
  Egg *egg1 = new Egg("Farm A", 100.0, 2.5, -0.5, false, 1.3);
  report(fout, "Egg constructor with negative transportation cost coefficient", false);
  delete egg1;
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg constructor with negative transportation cost coefficient", true);
}

try
{
  Egg *egg1 = new Egg("Farm A", 100.0, 2.5, 0.5, false, -1.3);
  report(fout, "Egg constructor with negative organic coefficient", false);
  delete egg1;
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg constructor with negative organic coefficient", true);
}

fout << "\nTesting calculatePrice, calculateTransportationCost and calculateProfit:" << std::endl;

// Not organic: price = count * priceCoeff = 200 * 2.5 = 500
//              transport = count * transportCoeff = 200 * 0.4 = 80
//              profit = 500 - 80 = 420
egg1 = new Egg("Farm C", 200.0, 2.5, 0.4, false, 5.0);
report(fout, "Egg calculatePrice (not organic)",              nearlyEqual(egg1->calculatePrice(), 500.0, EPSILON));
report(fout, "Egg calculateTransportationCost (not organic)", nearlyEqual(egg1->calculateTransportationCost(), 80.0, EPSILON));
// Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
report(fout, "Egg calculateProfit (not organic)",             nearlyEqual(egg1->calculateProfit(), 420.0, EPSILON));
delete egg1;

// Organic: price = count * organicCoeff = 200 * 5.0 = 1000
//          transport = 200 * 0.4 = 80
//          profit = 1000 - 80 = 920
egg1 = new Egg("Farm C", 200.0, 2.5, 0.4, true, 5.0);
report(fout, "Egg calculatePrice (organic)",              nearlyEqual(egg1->calculatePrice(), 1000.0, EPSILON));
report(fout, "Egg calculateTransportationCost (organic)", nearlyEqual(egg1->calculateTransportationCost(), 80.0, EPSILON));
// Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
report(fout, "Egg calculateProfit (organic)",             nearlyEqual(egg1->calculateProfit(), 920.0, EPSILON));
delete egg1;

fout << "\nTesting makeOrganic:" << std::endl;

egg1 = new Egg("Farm A", 100.0, 2.5, 0.5, false, 5.0);
report(fout, "Egg getIsOrganic before makeOrganic", egg1->getIsOrganic() == false);
egg1->makeOrganic();
report(fout, "Egg getIsOrganic after makeOrganic",  egg1->getIsOrganic() == true);
// price should now apply organicCoefficient: 100 * 5.0 = 500
report(fout, "Egg calculatePrice after makeOrganic", nearlyEqual(egg1->calculatePrice(), 500.0, EPSILON));
delete egg1;

// ── repurposePercentage ───────────────────────────────────────────────────────
fout << "\nTesting repurposePercentage:" << std::endl;

// count = 200, repurpose 10% → 200 * (1 - 0.10) = 200 * 0.90 = 180.0
egg1 = new Egg("Farm A", 200.0, 2.5, 0.5, false, 5.0);
egg1->repurposePercentage(10.0);
report(fout, "Egg repurposePercentage", nearlyEqual(egg1->getCount(), 180.0, EPSILON));
try
{
  egg1->repurposePercentage(-5.0);
  report(fout, "Egg repurposePercentage with negative value", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg repurposePercentage with negative value", true);
}
try
{
  egg1->repurposePercentage(150.0);
  report(fout, "Egg repurposePercentage with value greater than 100", false);
}
catch (const std::invalid_argument &e)
{
  report(fout, "Egg repurposePercentage with value greater than 100", true);
}
delete egg1;

// Operators compare by calculateProfit(). isOrganic=false so organicCoeff is irrelevant.
fout << "\nTesting comparison operators:" << std::endl;

// egg2 profit = 400*(2.0-0.4) = 640   egg3 profit = 500*(2.5-0.5) = 1000
fout << "Testing when egg2 is less than egg3:" << std::endl;
Egg *egg2 = new Egg("Farm D", 400.0, 2.0, 0.4, false, 1.0);
Egg *egg3 = new Egg("Farm E", 500.0, 2.5, 0.5, false, 1.0);
report(fout, "Egg operator==", (*egg2 == *egg3) == false);
report(fout, "Egg operator>",  (*egg2 > *egg3)  == false);
report(fout, "Egg operator<",  (*egg2 < *egg3)  == true);
report(fout, "Egg operator>=", (*egg2 >= *egg3) == false);
report(fout, "Egg operator<=", (*egg2 <= *egg3) == true);
delete egg2;
delete egg3;

// egg2 profit = 500*(2.5-0.5) = 1000   egg3 profit = 400*(2.0-0.4) = 640
fout << "Testing when egg2 is more than egg3:" << std::endl;
egg2 = new Egg("Farm D", 500.0, 2.5, 0.5, false, 1.0);
egg3 = new Egg("Farm E", 400.0, 2.0, 0.4, false, 1.0);
report(fout, "Egg operator==", (*egg2 == *egg3) == false);
report(fout, "Egg operator>",  (*egg2 > *egg3)  == true);
report(fout, "Egg operator<",  (*egg2 < *egg3)  == false);
report(fout, "Egg operator>=", (*egg2 >= *egg3) == true);
report(fout, "Egg operator<=", (*egg2 <= *egg3) == false);
delete egg2;
delete egg3;

// Both profit = 500*(2.5-0.5) = 1000
fout << "1. Testing when egg2 is equal to egg3:" << std::endl;
egg2 = new Egg("Farm D", 500.0, 2.5, 0.5, false, 1.0);
egg3 = new Egg("Farm E", 500.0, 2.5, 0.5, false, 1.0);
report(fout, "Egg operator==", (*egg2 == *egg3) == true);
report(fout, "Egg operator>",  (*egg2 > *egg3)  == false);
report(fout, "Egg operator<",  (*egg2 < *egg3)  == false);
report(fout, "Egg operator>=", (*egg2 >= *egg3) == true);
report(fout, "Egg operator<=", (*egg2 <= *egg3) == true);
delete egg2;
delete egg3;

// egg2: 500*(2.5-0.5)=1000   egg3: 1000*(1.5-0.5)=1000 — same profit, different params
fout << "2. Testing when egg2 is equal to egg3:" << std::endl;
egg2 = new Egg("Farm D",  500.0, 2.5, 0.5, false, 1.0);
egg3 = new Egg("Farm E", 1000.0, 1.5, 0.5, false, 1.0);
report(fout, "Egg operator==", (*egg2 == *egg3) == true);
report(fout, "Egg operator>",  (*egg2 > *egg3)  == false);
report(fout, "Egg operator<",  (*egg2 < *egg3)  == false);
report(fout, "Egg operator>=", (*egg2 >= *egg3) == true);
report(fout, "Egg operator<=", (*egg2 <= *egg3) == true);
delete egg2;
delete egg3;

fout << "\nTesting polymorphism:" << std::endl;

Product *product1 = new Egg("Farm A", 200.0, 2.5, 0.5, false, 1.3);
if (auto *e = dynamic_cast<Egg *>(product1))
{
  report(fout, "Egg dynamic_cast and getCount from Product class", nearlyEqual(e->getCount(), 200.0, EPSILON));
}
else
{
  report(fout, "Egg dynamic_cast and getCount from Product class", false);
}

Product *product2 = new Egg("Farm A", 200.0, 2.5, 0.5, false, 1.3);
if (auto *e = dynamic_cast<Egg *>(product2))
{
  e->setCount(150.0);
  report(fout, "Egg dynamic_cast and setCount", nearlyEqual(e->getCount(), 150.0, EPSILON));
}
else
{
  report(fout, "Egg dynamic_cast and setCount", false);
}

// price = 100 * 2.5 = 250,  transport = 100 * 0.5 = 50,  profit = 200
Product *product3 = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);
report(fout, "Egg calculatePrice through Product pointer",              nearlyEqual(product3->calculatePrice(), 250.0, EPSILON));
report(fout, "Egg calculateTransportationCost through Product pointer", nearlyEqual(product3->calculateTransportationCost(), 50.0, EPSILON));
// Template design pattern: Template method (calls calculatePrice and calculateTransportationCost)
report(fout, "Egg calculateProfit through Product pointer",             nearlyEqual(product3->calculateProfit(), 200.0, EPSILON));
delete product3;
delete product2;
delete product1;

fout << "\nTesting deep copy:" << std::endl;

{
  Egg *original = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);
  Egg *copy     = original->clone();
  report(fout, "Egg deep copy (getCount)",                         nearlyEqual(copy->getCount(), original->getCount(), EPSILON));
  report(fout, "Egg deep copy (getPriceCoefficient)",              nearlyEqual(copy->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
  report(fout, "Egg deep copy (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
  report(fout, "Egg deep copy (getIsOrganic)",                     copy->getIsOrganic() == original->getIsOrganic());
  report(fout, "Egg deep copy (getOrganicCoefficient)",            nearlyEqual(copy->getOrganicCoefficient(), original->getOrganicCoefficient(), EPSILON));
  delete original;
  delete copy;
}

{
  Egg *original = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);
  Egg *copy     = original->clone();
  original->setCount(200.0);
  report(fout, "Egg deep copy independence (getCount)",                        nearlyEqual(copy->getCount(), 100.0, EPSILON));
  original->setPriceCoefficient(3.0);
  report(fout, "Egg deep copy independence (getPriceCoefficient)",              nearlyEqual(copy->getPriceCoefficient(), 2.5, EPSILON));
  original->setTransportationCostCoefficient(0.8);
  report(fout, "Egg deep copy independence (getTransportationCostCoefficient)", nearlyEqual(copy->getTransportationCostCoefficient(), 0.5, EPSILON));
  original->setIsOrganic(true);
  report(fout, "Egg deep copy independence (getIsOrganic)",                     copy->getIsOrganic() == false);
  original->setOrganicCoefficient(2.0);
  report(fout, "Egg deep copy independence (getOrganicCoefficient)",            nearlyEqual(copy->getOrganicCoefficient(), 1.3, EPSILON));
  delete original;
  delete copy;
}

fout << "Testing clone method through Product pointer:" << std::endl;

{
  Product *original      = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);
  Product *clone         = original->clone();
  Egg     *clone_dynamic = dynamic_cast<Egg *>(clone);
  Egg     *orig_dynamic  = dynamic_cast<Egg *>(original);
  if (clone_dynamic && orig_dynamic)
  {
    report(fout, "Egg deep copy (getCount with dynamic_cast)", nearlyEqual(clone_dynamic->getCount(), orig_dynamic->getCount(), EPSILON));
  }
  else
  {
    report(fout, "Egg deep copy (getCount with dynamic_cast)", false);
  }
  report(fout, "Egg deep copy (getPriceCoefficient)",              nearlyEqual(clone->getPriceCoefficient(), original->getPriceCoefficient(), EPSILON));
  report(fout, "Egg deep copy (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), original->getTransportationCostCoefficient(), EPSILON));
  delete original;
  delete clone;
}

{
  Product *original      = new Egg("Farm A", 100.0, 2.5, 0.5, false, 1.3);
  Product *clone         = original->clone();
  Egg     *clone_dynamic = dynamic_cast<Egg *>(clone);
  Egg     *orig_dynamic  = dynamic_cast<Egg *>(original);
  if (clone_dynamic && orig_dynamic)
  {
    orig_dynamic->setCount(200.0);
    report(fout, "Egg deep copy independence (getCount with dynamic_cast)", nearlyEqual(clone_dynamic->getCount(), 100.0, EPSILON));
  }
  else
  {
    report(fout, "Egg deep copy independence (getCount with dynamic_cast)", false);
  }
  original->setPriceCoefficient(3.0);
  report(fout, "Egg deep copy independence (getPriceCoefficient)",              nearlyEqual(clone->getPriceCoefficient(), 2.5, EPSILON));
  original->setTransportationCostCoefficient(0.8);
  report(fout, "Egg deep copy independence (getTransportationCostCoefficient)", nearlyEqual(clone->getTransportationCostCoefficient(), 0.5, EPSILON));
  delete original;
  delete clone;
}
    */

  {
    fout << std::endl << "Testing ProductContainer class:" << std::endl;
    fout << "Testing default constructor and push_back:" << std::endl;
    ProductContainer container;
    container.push_back(new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2));
    report(fout, "ProductContainer default constructor and push_back", container.getSize() == 1);
    container.push_back(new Milk("Farm B", 200.0, 2.0, 0.4, true, 1.5));
    report(fout, "ProductContainer push_back", container.getSize() == 2);
    container.push_back(new Wheat("Field A", 300.0, 1.5, 0.3));
    report(fout, "ProductContainer push_back with different product type", container.getSize() == 3);
    container.push_back(new Milk("Farm C", 150.0, 2.8, 0.6, false, 1.3));
    report(fout, "ProductContainer push_back with another product type", container.getSize() == 4);
    //fout << container.toString() << std::endl;

    fout << std::endl << "Testing clear:" << std::endl;
    container.clear();
    report(fout, "1. ProductContainer clear", container.getSize() == 0);
    container.clear(); // Clear again to test clearing an already empty container
    report(fout, "2. ProductContainer clear on already empty container", container.getSize() == 0);
  }

  {
    fout << std::endl << "Testing begin and end iterators:" << std::endl;

    ProductContainer container;
    container.push_back(new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2));
    container.push_back(new Milk("Farm B", 200.0, 2.0, 0.4, true, 1.5));
    container.push_back(new Wheat("Farm C", 300.0, 1.5, 0.3));
    container.push_back(new Milk("Farm D", 150.0, 2.8, 0.6, false, 1.3));

    auto it = container.begin();
    report(fout, "ProductContainer begin iterator dereference", it != container.end() && (*it)->getOrigin() == "Farm A");
    ++it;
    report(fout, "ProductContainer iterator pre-increment in the line before dereference", it != container.end() && (*it)->getOrigin() == "Farm B");
    report(fout, "ProductContainer iterator pre-increment in the same line as dereference", it != container.end() && (*(++it))->getOrigin() == "Farm C");
    it++;
    report(fout, "ProductContainer iterator post-increment in the line before dereference", it != container.end() && (*it)->getOrigin() == "Farm D"); 
    report(fout, "ProductContainer iterator post-increment in the same line as dereference", it != container.end() && (*(it++))->getOrigin() == "Farm D");
    report(fout, "ProductContainer end iterator", it == container.end());
    try
    {
      *it; // Dereferencing end iterator should throw
      report(fout, "ProductContainer dereferencing end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer dereferencing end iterator should throw", true);
    }
    try
    {
      ++it; // Incrementing end iterator should throw
      report(fout, "ProductContainer pre-incrementing end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer pre-incrementing end iterator should throw", true);
    }
    try
    {
      it++; // Post-incrementing end iterator should throw
      report(fout, "ProductContainer post-incrementing end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer post-incrementing end iterator should throw", true);
    }
  }
  {
    ProductContainer container;
    container.push_back(new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2));
    container.push_back(new Milk("Farm B", 200.0, 2.0, 0.4, true, 1.5));
    auto it2 = container.begin();
    auto it3 = container.begin();
    report(fout, "ProductContainer iterator equality", it2 == it3);
    ++it2;
    report(fout, "ProductContainer iterator equality", !(it2 == it3));
    auto it4 = container.end();
    auto it5 = container.end();
    report(fout, "ProductContainer iterator equality", it4 == it5);

    it3 = it2;
    report(fout, "ProductContainer deep copy", it2 == it3);
    it3++;
    report(fout, "ProductContainer deep copy independence", it2 != it3);

  }

  {
    // pass by reference?
    fout << std::endl << "Testing insert:" << std::endl;
    ProductContainer container;
    container.insert(container.end(), new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2));
    report(fout, "ProductContainer insert at end", (*container.begin())->getOrigin() == "Farm A");
    
    container.insert(container.end(), new Milk("Farm B", 200.0, 2.0, 0.4, true, 1.5));
    auto it = container.begin();
    ++it;
    report(fout, "ProductContainer insert at end again", container.begin() != container.end() && (*it)->getOrigin() == "Farm B");
    
    container.insert(it, new Wheat("Farm C", 300.0, 1.5, 0.3));
    it = container.begin();
    ++it;
    report(fout, "ProductContainer insert at middle", container.begin() != container.end() && (*it)->getOrigin() == "Farm C");
    
    container.insert(container.begin(), new Milk("Farm D", 150.0, 2.8, 0.6, false, 1.3));
    report(fout, "ProductContainer insert at beginning", container.begin() != container.end() && (*container.begin())->getOrigin() == "Farm D");
    
    container.insert(container.begin(), new Milk("Farm E", 250.0, 2.3, 0.4, true, 1.4));
    report(fout, "ProductContainer insert at beginning again", container.begin() != container.end() && (*container.begin())->getOrigin() == "Farm E");
    
    report(fout, "ProductContainer size after inserts", container.getSize() == 5);

    fout << std::endl << "Testing get(ProductContainer::iterator)" << std::endl;
    Product* p = container.get(container.begin()); // don't delete, container owns it!!
    report(fout, "ProductContainer get", p != nullptr && p->getOrigin() == "Farm E");
    try
    {
      container.get(container.end()); // should throw
      report(fout, "ProductContainer get with end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer get with end iterator should throw", true);
    }
    it = container.begin();
    ++it;
    Product* p3 = container.get(it);
    report(fout, "ProductContainer get with middle iterator", p3 != nullptr && p3->getOrigin() == "Farm D");

    fout << std::endl << "Testing update(ProductContainer::iterator)" << std::endl;
    container.update(container.begin(), new Milk("Farm F", 300.0, 2.0, 0.5, false, 1.1));
    report(fout, "ProductContainer update at beginning", (*container.begin())->getOrigin() == "Farm F");
    it = container.begin();
    ++it;
    container.update(it, new Milk("Farm G", 400.0, 1.8, 0.4, true, 1.6));
    report(fout, "ProductContainer update at middle", (*it)->getOrigin() == "Farm G");
    try
    {
      container.update(container.end(), new Milk("Farm H", 500.0, 1.5, 0.3, false, 1.2)); // should throw
      report(fout, "ProductContainer update with end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer update with end iterator should throw", true);
    }
  }

  {
    fout << std::endl << "Testing remove(ProductContainer::iterator)" << std::endl;
    ProductContainer container;
    container.push_back(new Milk("Farm A", 100.0, 2.5, 0.5, false, 1.2));
    container.push_back(new Milk("Farm B", 200.0, 2.0, 0.4, true, 1.5));
    container.push_back(new Wheat("Farm C", 300.0, 1.5, 0.3));
    container.push_back(new Milk("Farm D", 150.0, 2.8, 0.6, false, 1.3));
    auto it = container.begin();
    container.remove(it);
    report(fout, "ProductContainer remove at beginning", container.begin() != container.end() && (*container.begin())->getOrigin() == "Farm B");
    it = container.begin();
    ++it;
    container.remove(it);
    report(fout, "ProductContainer remove at middle", container.begin() != container.end() && (*(++container.begin()))->getOrigin() == "Farm D");
    try
    {
      container.remove(container.end()); // should throw
      report(fout, "ProductContainer remove with end iterator should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer remove with end iterator should throw", true);
    }

    container.clear();
    report(fout, "ProductContainer clear", container.getSize() == 0);
    try
    {
      container.remove(container.begin()); // should throw on empty container
      report(fout, "ProductContainer remove with begin iterator on empty container should throw", false);
    }
    catch (const std::out_of_range &e)
    {
      report(fout, "ProductContainer remove with begin iterator on empty container should throw", true);
    }
    container.clear();
    report(fout, "ProductContainer clear again on already empty container", container.getSize() == 0);
  }

  }
  catch (...)
  {
    fout << "An unexpected exception occurred: " << std::endl;
  }

  fout << std::endl
       << "Total tests passed: " << passed << std::endl;
  fout << "Total tests failed: " << failed << std::endl;

  fout.close();
  return 0;
}

#endif // TEST_MODE