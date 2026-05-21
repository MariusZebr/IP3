COMP = g++
COMPFLAGS = -std=c++17 -Wall -Wextra -g -Wpedantic

# c) run demo
c: demo.exe
	./demo.exe

# b) build demo
b: demo.exe
	
# d) build test
d: test.exe

# e) run test
e: test.exe
	./test.exe
	
# f) clean everything
f:
	del *.o *.exe

# g) rebuild from scratch
g: f a

demo.exe: demo.o milk.o wheat.o Product.o DiscountedPrice.o RegularPrice.o TaxedPrice.o ProductContainer.o NotImplementedException.o StrategyNotSetException.o
	$(COMP) $(COMPFLAGS) -o demo.exe demo.o milk.o wheat.o Product.o DiscountedPrice.o RegularPrice.o TaxedPrice.o ProductContainer.o NotImplementedException.o StrategyNotSetException.o

test.exe: test.o milk.o wheat.o Product.o DiscountedPrice.o RegularPrice.o TaxedPrice.o ProductContainer.o NotImplementedException.o StrategyNotSetException.o
	$(COMP) $(COMPFLAGS) -o test.exe test.o milk.o wheat.o Product.o DiscountedPrice.o RegularPrice.o TaxedPrice.o ProductContainer.o NotImplementedException.o StrategyNotSetException.o

test.o: src/test.cpp include/Product.h include/Milk.h include/Wheat.h include/ProductContainer.h include/strategies/PricingStrategy.h include/strategies/DiscountedPrice.h include/exceptions/StrategyNotSetException.h include/exceptions/NotImplementedException.h
	$(COMP) $(COMPFLAGS) -DTEST_MODE -c src/test.cpp -o test.o

demo.o: src/demo.cpp include/Product.h include/Milk.h include/Wheat.h include/ProductContainer.h include/strategies/PricingStrategy.h include/strategies/DiscountedPrice.h include/exceptions/StrategyNotSetException.h include/exceptions/NotImplementedException.h
	$(COMP) $(COMPFLAGS) -DDEMO_MODE -c src/demo.cpp -o demo.o

milk.o: src/Milk.cpp include/Milk.h include/Product.h
	$(COMP) $(COMPFLAGS) -c src/Milk.cpp -o milk.o

wheat.o: src/Wheat.cpp include/Wheat.h include/Product.h
	$(COMP) $(COMPFLAGS) -c src/Wheat.cpp -o wheat.o

Product.o: src/Product.cpp include/Product.h
	$(COMP) $(COMPFLAGS) -c src/Product.cpp -o Product.o

DiscountedPrice.o: src/strategies/DiscountedPrice.cpp include/strategies/DiscountedPrice.h include/strategies/PricingStrategy.h
	$(COMP) $(COMPFLAGS) -c src/strategies/DiscountedPrice.cpp -o DiscountedPrice.o

RegularPrice.o: src/strategies/RegularPrice.cpp include/strategies/RegularPrice.h include/strategies/PricingStrategy.h
	$(COMP) $(COMPFLAGS) -c src/strategies/RegularPrice.cpp -o RegularPrice.o

TaxedPrice.o: src/strategies/TaxedPrice.cpp include/strategies/TaxedPrice.h include/strategies/PricingStrategy.h
	$(COMP) $(COMPFLAGS) -c src/strategies/TaxedPrice.cpp -o TaxedPrice.o

ProductContainer.o: src/ProductContainer.cpp include/ProductContainer.h include/Product.h include/strategies/PricingStrategy.h include/exceptions/StrategyNotSetException.h include/exceptions/NotImplementedException.h
	$(COMP) $(COMPFLAGS) -c src/ProductContainer.cpp -o ProductContainer.o

NotImplementedException.o: src/exceptions/NotImplementedException.cpp include/exceptions/NotImplementedException.h
	$(COMP) $(COMPFLAGS) -c src/exceptions/NotImplementedException.cpp -o NotImplementedException.o

StrategyNotSetException.o: src/exceptions/StrategyNotSetException.cpp include/exceptions/StrategyNotSetException.h
	$(COMP) $(COMPFLAGS) -c src/exceptions/StrategyNotSetException.cpp -o StrategyNotSetException.o