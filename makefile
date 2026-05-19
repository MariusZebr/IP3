demo.o: src/demo.cpp include/Product.h include/Milk.h include/Wheat.h include/ProductContainer.h include/strategies/PricingStrategy.h include/strategies/DiscountedPrice.h include/exceptions/StrategyNotSetException.h include/exceptions/NotImplementedException.h
	g++ -c src/demo.cpp -o demo.o
milk.o: src/Milk.cpp include/Milk.h include/Product.h
	g++ -c src/Milk.cpp -o milk.o
wheat.o: src/Wheat.cpp include/Wheat.h include/Product.h
	g++ -c src/Wheat.cpp -o wheat.o

product_container.o: src/ProductContainer.cpp include/ProductContainer.h include/Product.h include/strategies/PricingStrategy.h include/exceptions/StrategyNotSetException.h include/exceptions/NotImplementedException.h
	g++ -c src/ProductContainer.cpp -o product