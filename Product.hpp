#ifndef PRODUCT
#define PRODUCT

#include <iostream>
#include <string>
#include <vector>
#include <map>

static int next_key = 0;
enum class ProductCategory
{
    Product,
    Clothing,
    Electronics,
    Toys,
    SportingGoods,
    Furniture
};

class Product
{
public:
    Product(int key, std::string name, double price, int quality, std::string brand, int status) : product_key_(key), product_name_(name), base_price_(price), bid_price_(price), quality_(quality), brand_(brand), status_(status){};
    Product(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : product_key_(key), product_name_(name), base_price_(price), bid_price_(bid), quality_(quality), brand_(brand), status_(status){};

    virtual ProductCategory getCategory() { return ProductCategory::Product; };
    int getProductKey() const { return product_key_; }
    std::string getProductName() const { return product_name_; };
    double getPrice() const { return base_price_; };
    int getQuality() { return quality_; };
    void displayQuality();
    std::string getBrand() const { return brand_; };
    double getBid() { return bid_price_; };
    int getStatus() { return status_; };
    void displayStatus();

    void setProductKey() { product_key_ = ++next_key; }
    void setProductName(std::string product_name) { product_name_ = product_name; };
    void setPrice(int base_price) { base_price_ = base_price; };
    void setQuality(int quality) { quality_ = quality; };
    void setBrand(std::string brand) { brand_ = brand; };
    void setBid(double bid);
    void setStatus(int status);

    // int getNextKey() { return ++next_key;}

protected:
    int product_key_;
    std::string product_name_;
    double base_price_;
    double bid_price_;
    int quality_;
    std::string brand_;
    int status_;
};

void Product::displayQuality()
{
    std::string quality;
    switch (quality_)
    {
    case 1:
        std::cout << "New" << std::endl;
        break;

    case 2:
        std::cout << "Used-Very Good" << std::endl;
        break;

    case 3:
        std::cout << "Used-Good" << std::endl;
        break;

    case 4:
        std::cout << "Used-Okay" << std::endl;
        break;

    default:
        std::cout << "error: no quality given" << std::endl;
        break;
    }
}

void Product::displayStatus()
{
    std::string quality;
    switch (quality_)
    {
    case 1:
        std::cout << "Open Bid" << std::endl;
        break;

    case 2:
        std::cout << "Closed Bid - Unsold" << std::endl;
        break;

    case 3:
        std::cout << "Closed Bid - Sold" << std::endl;
        break;

    default:
        std::cout << "error: no status given" << std::endl;
        break;
    }
}

void Product::setBid(double bid)
{
    if (bid_price_ < bid)
    {
        bid_price_ = bid;
    }
}

void Product::setStatus(int status)
{
    if (status_ != 3)
    {
        status_ = status;
    }
    else
    {
        std::cerr << "error: cannot alter a sold product's status" << std::endl;
    }
}

//////////////////////////////////////////////////////////////
/////////////////////// CLOTHING CLASS ///////////////////////
//////////////////////////////////////////////////////////////
class Clothing : public Product
{
public:
    Clothing(int key, std::string name, double price, int quality, std::string brand, int status) : Product(key, name, price, quality, brand, status){};
    Clothing(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : Product(key, name, price, bid, quality, brand, status){};

    ProductCategory getCategory() override { return ProductCategory::Clothing; }

private:
};

/////////////////////////////////////////////////////////////////
/////////////////////// ELECTRONICS CLASS ///////////////////////
/////////////////////////////////////////////////////////////////
class Electronics : public Product
{
public:
    Electronics(int key, std::string name, double price, int quality, std::string brand, int status) : Product(key, name, price, quality, brand, status){};
    Electronics(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : Product(key, name, price, bid, quality, brand, status){};

    ProductCategory getCategory() override { return ProductCategory::Electronics; }

private:
};

//////////////////////////////////////////////////////////
/////////////////////// TOYS CLASS ///////////////////////
//////////////////////////////////////////////////////////
class Toys : public Product
{
public:
    Toys(int key, std::string name, double price, int quality, std::string brand, int status) : Product(key, name, price, quality, brand, status){};
    Toys(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : Product(key, name, price, bid, quality, brand, status){};

    ProductCategory getCategory() override { return ProductCategory::Toys; }

private:
};

////////////////////////////////////////////////////////////////////
/////////////////////// SPORTING GOODS CLASS ///////////////////////
////////////////////////////////////////////////////////////////////
class SportingGoods : public Product
{
public:
    SportingGoods(int key, std::string name, double price, int quality, std::string brand, int status) : Product(key, name, price, quality, brand, status){};
    SportingGoods(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : Product(key, name, price, bid, quality, brand, status){};

    ProductCategory getCategory() override { return ProductCategory::SportingGoods; }

private:
};

///////////////////////////////////////////////////////////////
/////////////////////// FURNITURE CLASS ///////////////////////
///////////////////////////////////////////////////////////////
class Furniture : public Product
{
public:
    Furniture(int key, std::string name, double price, int quality, std::string brand, int status) : Product(key, name, price, quality, brand, status){};
    Furniture(int key, std::string name, double price, double bid, int quality, std::string brand, int status) : Product(key, name, price, bid, quality, brand, status){};

    ProductCategory getCategory() override { return ProductCategory::Furniture; }

private:
};

///////////////////////////////////////////////////////////////
/////////////////////// FACTORY CLASS /////////////////////////
///////////////////////////////////////////////////////////////

class ProductFactory
{
public:
    Product *make_product(int choice, std::string name, double price, int quality, std::string brand, int status);
    Product *make_product(int choice, std::string name, double price, double bid, int quality, std::string brand, int status);
    Product *make_product(int choice, int key, std::string name, double price, double bid, int quality, std::string brand, int status);
};

Product *ProductFactory::make_product(int choice, std::string name, double price, int quality, std::string brand, int status)
{
    ProductCategory category_choice = ProductCategory(choice);
    int key = ++next_key;
    switch (choice)
    {
    case 1:
        return new Clothing(key, name, price, quality, brand, status);
        break;

    case 2:
        return new Electronics(key, name, price, quality, brand, status);
        break;

    case 3:
        return new Toys(key, name, price, quality, brand, status);
        break;

    case 4:
        return new SportingGoods(key, name, price, quality, brand, status);
        break;

    case 5:
        return new Furniture(key, name, price, quality, brand, status);
        break;

    default:
        std::cerr << "invalid choice" << std::endl;
        break;
    }
    return new Product(key, name, price, quality, brand, status);
}

Product *ProductFactory::make_product(int choice, std::string name, double price, double bid, int quality, std::string brand, int status)
{
    int key = ++next_key;
    switch (choice)
    {
    case 1:
        return new Clothing(key, name, price, bid, quality, brand, status);
        break;

    case 2:
        return new Electronics(key, name, price, bid, quality, brand, status);
        break;

    case 3:
        return new Toys(key, name, price, bid, quality, brand, status);
        break;

    case 4:
        return new SportingGoods(key, name, price, bid, quality, brand, status);
        break;

    case 5:
        return new Furniture(key, name, price, quality, brand, status);
        break;

    default:
        std::cerr << "invalid choice" << std::endl;
        break;
    }
    return new Product(key, name, price, bid, quality, brand, status);
}

Product *ProductFactory::make_product(int choice, int key, std::string name, double price, double bid, int quality, std::string brand, int status)
{
    switch (choice)
    {
    case 1:
        return new Clothing(key, name, price, bid, quality, brand, status);
        break;

    case 2:
        return new Electronics(key, name, price, bid, quality, brand, status);
        break;

    case 3:
        return new Toys(key, name, price, bid, quality, brand, status);
        break;

    case 4:
        return new SportingGoods(key, name, price, bid, quality, brand, status);
        break;

    case 5:
        return new Furniture(key, name, price, bid, quality, brand, status);
        break;

    default:
        std::cerr << "invalid choice" << std::endl;
        break;
    }
    return new Product(key, name, price, bid, quality, brand, status);
}

#endif